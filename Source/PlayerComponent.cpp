#include "PlayerComponent.h"


void PlayerComponent::sliderDragStarted(Slider* slider)
{
    if (slider == &musicSlider)
    {
        musicSliderBlockTimer = true;
    }
}

void PlayerComponent::sliderDragEnded(Slider* slider)
{
    if (slider == &musicSlider)
    {    
        double v = musicSlider.getValue();
        musicSliderBlockTimer = false;
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
        if (node != nullptr)
        {
            if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                {
                        plugin->setPosition(v);
                    
                }
            }
        }
    }
    
}


void PlayerComponent::sliderValueChanged(Slider* slider)
{

    if (slider == &volumeSlider)
    {
        currentVolume = 10*slider->getValue();
        if (currentVolume < 85)
        {
            volumeSlider.setColour(0x1001310, Colour((uint8_t)0x1d, (uint8_t)0xb9, (uint8_t)0x54, (float)currentVolume / 75));
        }
        else
        {
            volumeSlider.setColour(0x1001310, Colour((uint8_t)(0x1d + 7*(currentVolume-85)), (uint8_t)(0xb9 + 3*(currentVolume - 85)), (uint8_t)(0x54 - (currentVolume - 85)), (float)1));
        }

        if (isMuted && (currentVolume!=0))
        {
            updateVolumeButtonImage(false, currentVolume);
        }
        else if (currentVolume == 0)
        {
            updateVolumeButtonImage(true, currentVolume);
            isMuted = true;
        }
    }
//
    //if (slider == &energySlider)//1..10
    //{
    //    //glue reset
    //    if (abs(energySlider.getValue() - 5) < 0.31 )
    //    {
    //        energySlider.setValue(5, dontSendNotification);
    //    }
    //}
    
}


void PlayerComponent::CBScenesChanged(void)
    {
        switch (CBScenes.getSelectedId())
        {
            case 1: 
            {
                break;
            } 
            case 2: 
            {
                break;
            } 
            case 3: 
            {
                break;
            } 
            default: break;
        }
    }



void PlayerComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerComponent::paint(juce::Graphics& g)
{
    g.fillAll(Colour(WP_PLAYER));
}

void PlayerComponent::setAngle(float f)
{
    AudioProcessorGraph::Node* node;
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 4);
    if(node != nullptr)
    {
        if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
                {
                    /*
                    cartesian   0
                    azimuth     1    
                    elevation   2
                    distance    3
                    */
                    //plugin->setParameter(1,f);
                    if (auto* param = plugin->getParameters()[1])
                        return param->setValueNotifyingHost(f);
                    
                }
            }
    }
}

void PlayerComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    ///*
    //if (readerSource.get() == nullptr)
    //{
    //    bufferToFill.clearActiveBufferRegion();
    //    return;
    //}
    //mixerAudioSource.getNextAudioBlock(bufferToFill);
    //*/
    //auto level = (float) volumeSlider.getValue()/8;
    ////if (level == 0)
    ////{
    ////    return;
    ////}
    //mixerAudioSource.getNextAudioBlock(bufferToFill);
    //auto* device = deviceManager.getCurrentAudioDevice();
    //auto activeInputChannels  = device->getActiveInputChannels();
    //auto activeOutputChannels = device->getActiveOutputChannels();
    //auto maxInputChannels  = activeInputChannels .getHighestBit() + 1;
    //auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;
    //for (auto channel = 0; channel < maxOutputChannels; ++channel)
    //    {
    //        if ((! activeOutputChannels[channel]) || maxInputChannels == 0)
    //        {
    //            bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
    //        }
    //        else
    //        {
    //            auto actualInputChannel = channel % maxInputChannels; // [1]
    //            if (! activeInputChannels[channel]) // [2]
    //            {
    //                bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
    //            }
    //            else // [3]
    //            {
    //                auto* inBuffer = bufferToFill.buffer->getReadPointer (actualInputChannel,
    //                                                                      bufferToFill.startSample);
    //                auto* outBuffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
    //                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    //                {
    //                    outBuffer[sample] = inBuffer[sample] * level;
    //                }
    //            }
    //        }
    //    }
}

void PlayerComponent::releaseResources()
{
   // mixerAudioSource.releaseResources();
}

void PlayerComponent::resized()
{
    //energySlider.setBounds((getWidth() - 100) * 0.5f - 1, 5, 100, 80);
    playButton.setBounds((getWidth() - PLAY_BUTTON_SIZE) * 0.5f , 45 - 0.5* PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE); //aussi resized a updatePlayerButtonImage()
    prevButton.setBounds((getWidth() - NEXT_BUTTON_SIZE) * 0.5f - NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE);
    nextButton.setBounds((getWidth() - NEXT_BUTTON_SIZE) * 0.5f + NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE);
    musicSlider.setBounds(0.5 * (getWidth() - MUSIC_SLIDER_SIZE), 58, MUSIC_SLIDER_SIZE, 42);
    lengthLabel.setBounds(0.5 * (getWidth() + MUSIC_SLIDER_SIZE), 59, getWidth() - 20, 40);
    currentPositionLabel.setBounds(0.5 * (getWidth() - MUSIC_SLIDER_SIZE)-40, 59, getWidth() - 20, 40);

    playerTitlePlayingComponent.setBounds(10, 10, 200, 80);
    volumeSlider.setBounds(getWidth() - 200, 45, 150, 20);
    muteButton.setBounds(getWidth() - 200 - MUTE_BUTTON_SIZE, 45, MUTE_BUTTON_SIZE, MUTE_BUTTON_SIZE); //aussi resized a updateMuteButtonImage()
    cubeButton.setBounds(getWidth() - 200 - MUTE_BUTTON_SIZE - 50, 45, MUTE_BUTTON_SIZE, MUTE_BUTTON_SIZE);

    CBScenes.setBounds(185, 15,100,22);//22);

    DebugButton.setBounds(200,20,60,30);
    DebugButton2.setBounds(300,20,60,30);
}

void PlayerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{

}

void PlayerComponent::timerCallback()
{
    bool p = Master_isPlaying();
    double length_s;
    if(p)
    {
        if(Master_state != Master_Playing)
        {
            Master_changeState(Master_Playing);
        }


    AudioProcessorGraph::Node* node;
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
    if (node != nullptr)
        {
            if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                {
                    juce::RelativeTime position(plugin->getCurrentPosition_s());
                    auto minutes = ((int)position.inMinutes()) % 60;
                    auto seconds = ((int)position.inSeconds()) % 60;
                    auto positionString = juce::String::formatted("%02d:%02d", minutes, seconds);
                    currentPositionLabel.setText(positionString, juce::dontSendNotification);
                    if (!musicSliderBlockTimer)
                    {
                        musicSlider.setValue(10 * plugin->getCurrentPosition_s() / plugin->getLength_s(), dontSendNotification);
                    }
                }
            }
        }
    }
    else
    {
        if(Master_state == Master_Playing)
        {
            Master_changeState(Master_Stopped);
        }
    }


}

void PlayerComponent::Master_loadAndPlay(int idx)
{
    s_metadata md;
    if (!jsonParserLoad(idx, &md))
    {
        playerTitlePlayingComponent.loadSongData(md);
        currentIdxPlaying = idx; //todo optimiser   
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
        if (node != nullptr)
        {
            if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                {
                    plugin->loadAndPlay(idx,0);
                    playButton.setEnabled(true);
                    //energySlider.setEnabled(true);  
                    Master_changeState(Master_Starting);    
                    CBScenes.setVisible(true);                                  
                }
            }
        }
    }
    else
    {
        //song not found
    }
}

void PlayerComponent::Master_changeState(Master_TransportState Master_newState)
{
        int lengthDuration_s;
        int minutes; 
        int seconds;
        String LengthString;
        AudioProcessorGraph::Node* node;
        if (Master_state != Master_newState)
        {
            Master_state = Master_newState;
            switch (Master_state)
            {
            case Master_Stopped:
                updatePlayerButtonImage(false);
                node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
                if (node != nullptr)
                {
                    if (auto* processor = node->getProcessor())
                    {
                        if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                        {
                            plugin->changeState(CustomPlayerProcessor::Stopped);
                        }
                    }
                }
                break;
            case Master_Starting:
                updatePlayerButtonImage(true);
                node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
                if (node != nullptr)
                {
                    if (auto* processor = node->getProcessor())
                    {
                        if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                        {
                            plugin->changeState(CustomPlayerProcessor::Starting);
                            lengthDuration_s = plugin->getLength_s();
                            minutes = ((int)(lengthDuration_s / 60));
                            seconds = lengthDuration_s - (60 * minutes);
                            LengthString = juce::String::formatted("%02d:%02d", minutes, seconds);
                            lengthLabel.setText(LengthString, juce::dontSendNotification);
                            musicSlider.setValue(0, dontSendNotification);
                        }
                    }
                }
                break;
            case Master_Playing:
                updatePlayerButtonImage(true);
                break;
            case Master_Stopping:
                updatePlayerButtonImage(false);
                node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
                if (node != nullptr)
                {
                    if (auto* processor = node->getProcessor())
                    {
                        if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                        {
                            plugin->changeState(CustomPlayerProcessor::Stopping);
                        }
                    }
                }
                break;
            }
        }
}

bool PlayerComponent::Master_isPlaying(void)
{
    bool isPlaying = false;
    AudioProcessorGraph::Node* node;
   node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 3); //3=player0
    if (node != nullptr)
        {
            if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                {
                    isPlaying = plugin->isPlaying(); 

                }
            }
        }
        return isPlaying;
}


void PlayerComponent::updatePlayerButtonImage(bool playing)
{
    if (playing)
    {
        Image img2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"playbutton.png"));
        Image img = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"pauseButton.png"));
        playButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::transparentBlack,
            img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
            0.5f
        );
    }
    else
    {
        Image img = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"playbutton.png"));
        Image img2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"pauseButton.png"));
        playButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::transparentBlack,
            img2, 1.0f, juce::Colours::transparentBlack,
            0.5f
        );
    }
    playButton.setBounds((getWidth() - PLAY_BUTTON_SIZE) * 0.5f, 45 - 0.5 * PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE);
}

void PlayerComponent::updateVolumeButtonImage(bool isMuted, int sliderVolume)
{
    if (isMuted)
    {
        Image img = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"soundMuted.png"));
        Image img2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"soundMuted2.png"));
        muteButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::red,
            img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
            0.5f
        );
    }
    else
    {
        Image img = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"sound.png"));
        Image img2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"sound2.png"));
        muteButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::green,
            img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
            0.5f
        );
    }
    muteButton.setBounds(getWidth() - 200 - MUTE_BUTTON_SIZE, 45, MUTE_BUTTON_SIZE, MUTE_BUTTON_SIZE);
}


void PlayerComponent::muteButtonClicked(void)
{
    //if (isMuted)
    //{
    //    updateVolumeButtonImage(false, currentVolume);
    //    isMuted = false;
    //    currentVolume = volumeBeforeMute;;
    //    volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
    //}
    //else
    //{
    //    updateVolumeButtonImage(true, currentVolume);
    //    isMuted = true;
    //    volumeBeforeMute = currentVolume;
    //    currentVolume = 0;
    //    volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
    //}
}

void PlayerComponent::cubeButtonClicked(void)
{

}


void PlayerComponent::playButtonClicked()
{
    if (Master_isPlaying())
    {
        Master_changeState(Master_Stopping);
    }
    else
    {
        Master_changeState(Master_Starting);
    }
}


void PlayerComponent::nextButtonClicked(void)
{
    
    //if (currentIdxPlaying == jsonParserGetNbSong() - 1) //dernier son jou�, retour au premier
    //{
    //    Master_loadAndPlay(0);
    //    currentIdxPlaying = 0;
    //}
    //else
    //{
    //    Master_loadAndPlay(++currentIdxPlaying);
    //}
    
}

void PlayerComponent::prevButtonClicked(void)
{
    
    //if (currentIdxPlaying == 0) //dernier son joué, retour au premier
    //{
    //    currentIdxPlaying = jsonParserGetNbSong() - 1;
    //    Master_loadAndPlay(currentIdxPlaying);
    //}
    //else
    //{
    //    Master_loadAndPlay(--currentIdxPlaying);
    //}
    
}


static std::unique_ptr<ScopedDPIAwarenessDisabler> makeDPIAwarenessDisablerForPlugin (const PluginDescription& desc)
{
    //pas de resize de plugin
    return nullptr;
}


void PlayerComponent::addPluginCallback(std::unique_ptr<AudioPluginInstance> instance,  const String& error)
{
    if (instance == nullptr)
    {
        AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,TRANS("Error loading pluging"), error);
    }
    else
    {
        instance->enableAllBuses();
        auto VR3Node = mainProcessor->addNode(std::move(instance));
        for (int channel = 0; channel < 1; ++channel)     //connect_audio_node input/output
        {
            mainProcessor->addConnection({ { slots.getUnchecked(0)->nodeID,  channel }, { VR3Node->nodeID, channel } });
        }
        for (int channel = 0; channel < 2; ++channel)     //connect_audio_node input/output
        {
            mainProcessor->addConnection({ { VR3Node->nodeID,  channel }, { audioOutputNode->nodeID, channel } });
        }
        
            //node->properties.set ("x", pos.x);
            //node->properties.set ("y", pos.y);
            //changed();
        //}


        //init parameter
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 4); //vst3 dearvr pro
        if(node != nullptr)
        {
            if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
                    {
                        /*
                        cartesian   0
                        azimuth     1    
                        elevation   2
                        distance    3
                        */
                        //plugin->setParameter(1,f);
                        if (auto* param = plugin->getParameters()[0])
                            return param->setValue(1);

                    }
                }
        }
    }
}

/*
Processors can be added to the graph as "nodes" using addNode(), and once added,
 you can connect any of their input or output channels to other nodes using addConnection().
 */
void PlayerComponent::initialiseGraph(void)
{
    mainProcessor->clear();

    //NODE IN OUT 
    audioInputNode  = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioOutputNode));

    slots.add(slot1Node);
    slots.add(slot2Node);
    slots.set(0, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(0))); //0 : id du player
    //auto osc1 = std::make_unique<OscillatorProcessor>();
    //slots.set(0, mainProcessor->addNode(oscillator1));

    auto slot = slots.getUnchecked(0); //useless mais pour tester
    if (slot != nullptr)
    {
        if (slot->getProcessor()->getName() == "Player")
        {
            //config node
            slot->getProcessor()->setPlayConfigDetails(mainProcessor->getMainBusNumInputChannels(),
                mainProcessor->getMainBusNumOutputChannels(),
                mainProcessor->getSampleRate(),
                mainProcessor->getBlockSize());
        }
    }

    //https://forum.juce.com/t/connect-to-a-vst-plugin/20929/6
    OwnedArray<PluginDescription> pluginDescriptions;
    KnownPluginList plist;
    //AudioPluginFormatManager pluginFormatManager;
    vstformatManager.addDefaultFormats();
    for (int i = 0; i < vstformatManager.getNumFormats(); ++i)
    {
        plist.scanAndAddFile("C:/Users/Alex/Desktop/Coda2022/lsd_repo/dearVRpro.vst3", true, pluginDescriptions,  *vstformatManager.getFormat(i));
    }
    jassert (pluginDescriptions.size() > 0);
    String msg ("Oh no!");
    std::shared_ptr<ScopedDPIAwarenessDisabler> dpiDisabler = makeDPIAwarenessDisablerForPlugin(*pluginDescriptions[0]);
 
    vstformatManager.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error);
                                                                }
                                                            );
     

    //auto instance=  vstformatManager.createPluginInstance(*pluginDescriptions[0],
    //                                                     mainProcessor->getSampleRate(),
    //                                                     mainProcessor->getBlockSize(),
    //                                                     msg
    //                                                     );
  //
//

    //auto editor = instance->createEditor();
    //auto bc = editor->getConstrainer();
    //editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
    //addAndMakeVisible (editor);


    //String message;
    //KnownPluginList plugList;
    //AudioPluginFormatManager pluginFormatManager;
    //OwnedArray<juce::PluginDescription> typesFound;
    //juce::VSTPluginFormat format;
    //String pluginLoadError;
    //
    //message << "C:/Users/Alex/Desktop/Coda2022/lsd_repo/FruityPhaser.dll";
    //pluginFormatManager.addDefaultFormats();
    //plugList.scanAndAddFile(message, true, typesFound, format); // error happens here
    //auto test = pluginFormatManager.createPluginInstance(*typesFound[0], 44100, 512, pluginLoadError);


    //// Load VST //
    //PluginDescription PlugDesc;
    //PlugDesc.pluginFormatName = String("VST");
    ////PlugDesc.fileOrIdentifier = String("C:/Users/Alex/Desktop/Coda2022/lsd_repo/dearVRpro.vst3");
    //PlugDesc.fileOrIdentifier = String("C:/Users/Alex/Desktop/Coda2022/lsd_repo/FruityPhaser.dll");
    //std::shared_ptr<ScopedDPIAwarenessDisabler> dpiDisabler = makeDPIAwarenessDisablerForPlugin(PlugDesc);
    //vstformatManager->createPluginInstanceAsync(PlugDesc,
    //                                            mainProcessor->getSampleRate(),
    //                                            mainProcessor->getBlockSize(),
    //                                            [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
    //                                            {
    //                                            addPluginCallback(std::move(instance), error);
    //                                            }
    //);
//

    //CONNECT
    //clear
    //for (auto connection : mainProcessor->getConnections())
    //    mainProcessor->removeConnection (connection);


    //gestion du byas
    //for (int i = 0; i < 3; ++i)
    //{
    //    auto  slot   = slots   .getUnchecked (i);
    //    auto& bypass = bypasses.getReference (i);
    //
    //    if (slot != nullptr)
    //        slot->setBypassed (bypass->getToggleState());
    //}
}