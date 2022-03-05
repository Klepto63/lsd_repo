#pragma once

using namespace juce;

#include "JuceHeader.h"
#include "codaFront.h"
#include "Path.h"

class TableComponent : public Component, public TableListBoxModel

{
public:

    TableComponent()
    {
        // Load some data from an embedded XML file..
        loadData();

        // Create our table component and add it to this component..
        addAndMakeVisible(table);
        table.addMouseListener(this, true);
        table.setModel(this);
        selectedRow = 99;
        table.setRowHeight(50);
       
        table.setColour(ListBox::backgroundColourId, Colour(ROW_ALTERNATE_COLOR2));
        table.setOutlineThickness(0);



        // Add some columns to the table header, based on the column list in our database..
        forEachXmlChildElement(*columnList, columnXml)
        {
            table.getHeader().addColumn(columnXml->getStringAttribute("name"),
                columnXml->getIntAttribute("columnId"),
                columnXml->getIntAttribute("width"),
                50, 400,
                TableHeaderComponent::defaultFlags);
        }

        // we could now change some initial settings..
        table.getHeader().setSortColumnId(1, true); // sort forwards by the ID column
        table.getHeader().setColumnVisible(7, false); // hide the "length" column until the user shows it

        // un-comment this line to have a go of stretch-to-fit mode
        // table.getHeader().setStretchToFitActive (true);

        table.setMultipleSelectionEnabled(true);

    }

    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows() override
    {
        return numRows;
    }

    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
 
        if (rowNumber % 2)
            g.fillAll(Colour(ROW_ALTERNATE_COLOR2));
        else
            g.fillAll(Colour(ROW_ALTERNATE_COLOR));
        if (rowNumber == selectedRow)
        {
            g.fillAll(Colour(SELECTED_ROW_COLOR));
        }
   
    }

    void setselectedRow(int i)
    {
        selectedRow = i;
    }

    void selectedRowsChanged(int lastRowSelected) override
    {
        selectedRow = lastRowSelected;
    }

    int getRowId(void)
    {
        return selectedRow;
    }


    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell(Graphics& g, int rowNumber, int columnId,
        int width, int height, bool /*rowIsSelected*/) override
    {
        g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
        g.setFont(font);

        if (auto* rowElement = dataList->getChildElement(rowNumber))
        {
            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
            g.setFont(font);
            g.setColour(Colour(COLOR_TEXT_TABLE));
            g.drawText(text, 2, 0, width - 4, height, Justification::centred, true);
        }

        g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }

    // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
    // to change the sort order.
    void sortOrderChanged(int newSortColumnId, bool isForwards) override
    {
        if (newSortColumnId != 0)
        {
            DemoDataSorter sorter(getAttributeNameForColumnId(newSortColumnId), isForwards);
            dataList->sortChildElements(sorter);
            table.updateContent();
        }
    }

    // This is overloaded from TableListBoxModel, and must update any custom components that we're using
    Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/,
        Component* existingComponentToUpdate) override
    {
        if (columnId == 1 || columnId == 7) // The ID and Length columns do not have a custom component
        {
            jassert(existingComponentToUpdate == nullptr);
            return nullptr;
        }

        if (columnId == 5) // For the ratings column, we return the custom combobox component
        {
            auto* ratingsBox = static_cast<RatingColumnCustomComponent*> (existingComponentToUpdate);

            // If an existing component is being passed-in for updating, we'll re-use it, but
            // if not, we'll have to create one.
            if (ratingsBox == nullptr)
                ratingsBox = new RatingColumnCustomComponent(*this);

            ratingsBox->setRowAndColumn(rowNumber, columnId);
            return ratingsBox;
        }

        // The other columns are editable text columns, for which we use the custom Label component
        auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

        // same as above...
        if (textLabel == nullptr)
            textLabel = new EditableTextCustomComponent(*this);

        textLabel->setRowAndColumn(rowNumber, columnId);
        return textLabel;
    }

    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth(int columnId) override
    {
        if (columnId == 5)
            return 100; // (this is the ratings column, containing a custom combobox component)

        int widest = 32;

        // find the widest bit of text in this column..
        for (int i = getNumRows(); --i >= 0;)
        {
            if (auto* rowElement = dataList->getChildElement(i))
            {
                auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));

                widest = jmax(widest, font.getStringWidth(text));
            }
        }

        return widest + 8;
    }

    // A couple of quick methods to set and get cell values when the user changes them
    int getRating(const int rowNumber) const
    {
        return dataList->getChildElement(rowNumber)->getIntAttribute("Rating");
    }

    void setRating(const int rowNumber, const int newRating)
    {
        dataList->getChildElement(rowNumber)->setAttribute("Rating", newRating);
    }

    String getText(const int columnNumber, const int rowNumber) const
    {
        return dataList->getChildElement(rowNumber)->getStringAttribute(getAttributeNameForColumnId(columnNumber));
    }

    void setText(const int columnNumber, const int rowNumber, const String& newText)
    {
        auto columnName = table.getHeader().getColumnName(columnNumber);
        dataList->getChildElement(rowNumber)->setAttribute(columnName, newText);
    }

    //==============================================================================
    void resized() override
    {
        // position our table with a gap around its edge
        table.setBoundsInset(BorderSize<int>(8));
    }


private:
    TableListBox table;     // the table component itself
    Font font{ 18.0f };

    std::unique_ptr<XmlElement> demoData;  // This is the XML document loaded from the embedded file "demo table data.xml"
    XmlElement* columnList = nullptr;     // A pointer to the sub-node of demoData that contains the list of columns
    XmlElement* dataList = nullptr;     // A pointer to the sub-node of demoData that contains the list of data rows
    int numRows;                          // The number of rows of data we've got
    int selectedRow;

    //==============================================================================
    // This is a custom Label component, which we use for the table's editable text columns.
    class EditableTextCustomComponent : public Label
    {
    public:
        EditableTextCustomComponent(TableComponent& td) : owner(td)
        {
            // double click to edit the label text; single click handled below
            setEditable(false, false, false);
            owner.setComponentID("69");
        }

        void mouseDown(const MouseEvent& event) override
        {
            // single click on the label should simply select the row
            owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);
            Label::mouseDown(event);
    
        }

        void textWasEdited() override
        {
            owner.setText(columnId, row, getText());
        }

        // Our demo code will call this when we may need to update our contents
        void setRowAndColumn(const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            setText(owner.getText(columnId, row), dontSendNotification);
        }

        void paint(Graphics& g) override
        {
            auto& lf = getLookAndFeel();
           
            lf.setColour(textColourId, Colour(COLOR_TEXT_TABLE));

            Label::paint(g);
        }

    private:
        TableComponent& owner;
        int row, columnId;
        Colour textColour;
    };

    //==============================================================================
    // This is a custom component containing a combo box, which we're going to put inside
    // our table's "rating" column.
    class RatingColumnCustomComponent : public Component
    {
    public:
        RatingColumnCustomComponent(TableComponent& td) : owner(td)
        {
            // just put a combo box inside this component
            addAndMakeVisible(comboBox);
            comboBox.addItem("Original", 1);
            comboBox.addItem("Preset 1", 2);
            comboBox.addItem("Preset 2", 3);
            comboBox.addItem("Preset 3", 4);
            comboBox.addItem("Preset 4", 5);

            comboBox.onChange = [this] { owner.setRating(row, comboBox.getSelectedId()); };
            comboBox.setWantsKeyboardFocus(false);
        }

        void resized() override
        {
            comboBox.setBoundsInset(BorderSize<int>(2));
        }

        // Our demo code will call this when we may need to update our contents
        void setRowAndColumn(int newRow, int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            comboBox.setSelectedId(owner.getRating(row), dontSendNotification);
        }

    private:
        TableComponent& owner;
        ComboBox comboBox;
        int row, columnId;
    };

    //==============================================================================
    // A comparator used to sort our data when the user clicks a column header
    class DemoDataSorter
    {
    public:
        DemoDataSorter(const String& attributeToSortBy, bool forwards)
            : attributeToSort(attributeToSortBy),
            direction(forwards ? 1 : -1)
        {
        }

        int compareElements(XmlElement* first, XmlElement* second) const
        {
            auto result = first->getStringAttribute(attributeToSort)
                .compareNatural(second->getStringAttribute(attributeToSort));

            if (result == 0)
                result = first->getStringAttribute("ID")
                .compareNatural(second->getStringAttribute("ID"));

            return direction * result;
        }

    private:
        String attributeToSort;
        int direction;
    };

    //==============================================================================
    // this loads the embedded database XML file into memory
    void loadData()
    {
        //demoData = parseXML(File("C:/Users/Desktop/Coda2022/lsd_repo/Coda2022/tabledata.xml"));
        demoData = parseXML(File(ABS_PATH + (juce::String) "tabledata.xml"));

        dataList = demoData->getChildByName("DATA");
        columnList = demoData->getChildByName("COLUMNS");

        numRows = dataList->getNumChildElements();
    }

    // (a utility method to search our XML for the attribute that matches a column ID)
    String getAttributeNameForColumnId(const int columnId) const
    {
        forEachXmlChildElement(*columnList, columnXml)
        {
            if (columnXml->getIntAttribute("columnId") == columnId)
                return columnXml->getStringAttribute("name");
        }

        return {};
    }



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TableComponent)
};
























