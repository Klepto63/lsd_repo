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
        for(int ii = 0 ; ii  < MAX_INSTR ; ii++)
        {
            node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
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
    
}

void PlayerComponent::sliderValueChanged(Slider* slider)
{

    if (slider == &volumeSlider)
    {
        currentVolume = 10*slider->getValue();

        if (currentVolume < 75)
        {
            volumeSlider.setColour(0x1001310, Colour((uint8_t)0xEE, (uint8_t)0xEE, (uint8_t)0xEE, (float)currentVolume / 55));
        }
        else
        {
            //volumeSlider.setColour(0x1001310, Colour((uint8_t)(0x1d + 9*(currentVolume-50)), (uint8_t)(0xb9 + 3*(currentVolume - 85)), (uint8_t)(0x54 - 2 * (currentVolume - 85)), (float)1));
            volumeSlider.setColour(0x1001310, Colour( (uint8_t)(0xEE + (currentVolume/10)), (uint8_t)0xEE - 5*(currentVolume-75), (uint8_t)0xEE - 5*(currentVolume-75 ), (float) 1));
        }

        if (isMuted && (currentVolume!=0))
        {
            isMuted = false;
        }
        else if (currentVolume == 0)
        {
            isMuted = true;
        }
        updateVolumeButtonImage(isMuted, currentVolume);


        AudioProcessorGraph::Node* node;
        for(int ii = 0 ; ii  < MAX_INSTR ; ii++)
        {
            node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
            if (node != nullptr)
            {
                if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                    {
                            float gain = (1.0f*currentVolume) / 90.0f;
                            plugin->setGain((gain)); //1.0 = 0dB, 0.5 = -6dB, 2.0 = 6dB, etc.

                    }
                }
            }
        }

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

    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 8); //7=dearPro1
    if (node != nullptr)
    {
        if (auto* processor = node->getProcessor())
        {
            if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
            {
                if (auto* param = plugin->getParameters()[1])
                {
                    param->setValueNotifyingHost(0.25 + (f/2)  - 0.15);   //0..1 == -180..180
                }
            }
        }
    }
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 9); //7=dearPro1
    if (node != nullptr)
    {
        if (auto* processor = node->getProcessor())
        {
            if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
            {
                if (auto* param = plugin->getParameters()[1])
                {
                    param->setValueNotifyingHost(0.25 + (f/2)  );   //0..1 == -180..180
                }
            }
        }
    }
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 10); //7=dearPro1
    if (node != nullptr)
    {
        if (auto* processor = node->getProcessor())
        {
            if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
            {
                if (auto* param = plugin->getParameters()[1])
                {
                    param->setValueNotifyingHost(0.25 + (f/2)  );   //0..1 == -180..180
                }
            }
        }
    }
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 11); //7=dearPro1
    if (node != nullptr)
    {
        if (auto* processor = node->getProcessor())
        {
            if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
            {
                if (auto* param = plugin->getParameters()[1])
                {
                    param->setValueNotifyingHost(0.25 + (f/2)  );   //0..1 == -180..180
                }
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

    int Height = getHeight();
    int Width = getWidth();
    int h_playbar = 100; //largeur playbar like spotify
    //energySlider.setBounds((getWidth() - 100) * 0.5f - 1, 5, 100, 80);


    playButton.setBounds((Width - PLAY_BUTTON_SIZE) * 0.5f , 47 - 0.5* PLAY_BUTTON_SIZE - 6, PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE);   //aussi resized a updatePlayerButtonImage()


    prevButton.setBounds((Width - NEXT_BUTTON_SIZE_X) * 0.5f - NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE_X - 6, NEXT_BUTTON_SIZE_X, NEXT_BUTTON_SIZE_Y);
    nextButton.setBounds((Width - NEXT_BUTTON_SIZE_X) * 0.5f + NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE_X - 6, NEXT_BUTTON_SIZE_X, NEXT_BUTTON_SIZE_Y);
    repeatButton.setBounds((Width - NEXT_BUTTON_SIZE_X) * 0.5f - (3.0f * NEXT_BUTTON_SPACE), 45 - 0.5 * NEXT_BUTTON_SIZE_X - 6, NEXT_BUTTON_SIZE_X, NEXT_BUTTON_SIZE_Y);
    randomButton.setBounds((Width - NEXT_BUTTON_SIZE_X) * 0.5f - (2.0f * NEXT_BUTTON_SPACE), 45 - 0.5 * NEXT_BUTTON_SIZE_X - 6 , NEXT_BUTTON_SIZE_X, NEXT_BUTTON_SIZE_Y);


    playerTitlePlayingComponent.setBounds(20, 20, 200, 60);


    int musicSliderSize = 0.30f*Width + 150;

    currentPositionLabel.setBounds(0.5 * (Width - musicSliderSize) - 40, 59, Width - 20, 40);
    lengthLabel.setBounds(0.5 * (Width + musicSliderSize), 59, Width - 20, 40);

    musicSlider.setBounds(0.5 * (Width - musicSliderSize), 72, musicSliderSize, 15);

    muteButton.setBounds(0.8f*Width + 2 * ICON_BUTTON_SIZE, 0.5f*(h_playbar-ICON_BUTTON_SIZE), ICON_BUTTON_SIZE, ICON_BUTTON_SIZE); //aussi resized a updateMuteButtonImage()


    volumeSlider.setBounds(0.8f*Width + 3 * ICON_BUTTON_SIZE, 0.5f*(h_playbar-ICON_BUTTON_SIZE )+5, 0.15f*Width - 3 * ICON_BUTTON_SIZE , 15);




 
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

            if(repeatMode)
            {
                Master_loadAndPlay(currentIdxPlaying);
            }
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
        for(int ii = 0; ii < MAX_INSTR ; ii++)
        {
            node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
            if (node != nullptr)
            {
                if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                    {
                        plugin->loadAndPlay(idx,ii); //0 = stem 0
          
                    }
                }
            }
        }
        playButton.setEnabled(true);
        prevButton.setEnabled(true);
        nextButton.setEnabled(true);
        cubeButton.setEnabled(true);
        randomButton.setEnabled(true);
        repeatButton.setEnabled(true);
        musicSlider.setVisible(true);
        muteButton.setVisible(true);
        volumeSlider.setVisible(true);
        Master_changeState(Master_Starting);  
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
            {
                updatePlayerButtonImage(false);
                for(int ii = 0; ii < MAX_INSTR; ii++)
                {
                    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID)(3+ii)); //3=player0
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
                }
            }
                break;
            case Master_Starting:
                updatePlayerButtonImage(true);
                for(int ii = 0; ii < MAX_INSTR; ii++)
                {
                    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
                    if (node != nullptr)
                    {
                        if (auto* processor = node->getProcessor())
                        {
                            if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                            {
                                plugin->changeState(CustomPlayerProcessor::Starting);
                                if(ii==0)
                                {
                                    lengthDuration_s = plugin->getLength_s();
                                    minutes = ((int)(lengthDuration_s / 60));
                                    seconds = lengthDuration_s - (60 * minutes);
                                    LengthString = juce::String::formatted("%02d:%02d", minutes, seconds);
                                    lengthLabel.setText(LengthString, juce::dontSendNotification);
                                    musicSlider.setValue(0, dontSendNotification);
                                }
                            }
                        }
                    }
                }
                break;
            case Master_Playing:
                updatePlayerButtonImage(true);
                break;
            case Master_Stopping:
                updatePlayerButtonImage(false);
                for(int ii = 0; ii < MAX_INSTR; ii++)
                {
                    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
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
                }
                break;
            }
        }
}

void PlayerComponent::updateConfig(void)
{
    
}

void PlayerComponent::applyHightlight(void)
{
    AudioProcessorGraph::Node* node;
    for(int ii = 0; ii < MAX_INSTR ; ii++)
    {
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (8+ii)); //7=dearPro1
        if (node != nullptr)
        {
            if (auto* processor = node->getProcessor())
            {
                if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
                {
                    if (auto* param = plugin->getParameters()[7]) //30
                    {
                        float v =  sceneconfig_get_hightlight(ii);
                        param->setValueNotifyingHost(  v  );   //0..1 == -180..180
                    }
                }
            }
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
        Image img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_PAUSE_BUTTON)));
        Image img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_PAUSE_BUTTON_ONCLICK)));
        playButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::transparentBlack,
            img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
            0.5f
        );
    }
    else
    {
        Image img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_PLAY_BUTTON)));
        Image img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_PLAY_BUTTON_ONCLICK)));
        playButton.setImages(true, true, true,
            img, 1.0f, juce::Colours::transparentBlack,
            img, 1.0f, juce::Colours::transparentBlack,
            img2, 1.0f, juce::Colours::transparentBlack,
            0.5f
        );
    }
    resized();
}

void PlayerComponent::updateVolumeButtonImage(bool isMuted, int sliderVolume)
{
    Image img, img2;
    if (isMuted)
    {
        img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND_MUTED)));
        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND_MUTED_ONCLICK)));

    }
    else
    {
        if(sliderVolume < 30)
        {
            img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND1)));
        }
        else if(sliderVolume < 70)
        {
            img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND2)));
        }
        else
        {
            img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND3)));
        }

        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SOUND_ONCLICK)));
    }

     muteButton.setImages(true, true, true,
         img, 1.0f, juce::Colours::transparentBlack,
         img, 1.0f, juce::Colours::white,
         img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
         0.5f
     );
    resized();
}


void PlayerComponent::muteButtonClicked(void)
{
    if (isMuted)
    {
        isMuted = false;
        currentVolume = volumeBeforeMute;;
        volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
        updateVolumeButtonImage(false, currentVolume);
    }
    else
    {
        isMuted = true;
        volumeBeforeMute = currentVolume;
        currentVolume = 0;
        updateVolumeButtonImage(true, currentVolume);
        volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
    }

    if(Master_isPlaying())
    {
        AudioProcessorGraph::Node* node;
        for(int ii = 0 ; ii  < MAX_INSTR ; ii++)
        {
            node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) (3+ii)); //3=player0
            if (node != nullptr)
            {
                if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<CustomPlayerProcessor*> (processor))
                    {
                            float gain = (1.0f*currentVolume) / 90.0f;
                            plugin->setGain((gain)); //1.0 = 0dB, 0.5 = -6dB, 2.0 = 6dB, etc.

                    }
                }
            }
        }
    }
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

void PlayerComponent::repeatButtonClicked(void)
{
    Image img, img2;
    if(!repeatMode)
    {
        repeatMode = true;
        img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_REPEAT_BUTTON_CLICK)));
        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_REPEAT_BUTTON_CLICKON)));
    }
    else
    {
        img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_REPEAT_BUTTON)));
        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_REPEAT_BUTTON_ONCLICK)));
        repeatMode = false;
    }

    repeatButton.setImages(true, true, true,
        img, 1.0f, juce::Colours::transparentBlack,
        img, 1.0f, juce::Colours::transparentBlack,
        img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
        0.5f );
    resized();
}

void PlayerComponent::randomButtonClicked(void)
{
    Image img, img2;
    if(!randomMode)
    {
        randomMode = true;
        img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_RANDOM_BUTTON_CLICK)));
        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_RANDOM_BUTTON_CLICKON)));
    }
    else
    {
        img = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_RANDOM_BUTTON)));
        img2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_RANDOM_BUTTON_ONCLICK)));
        randomMode = false;
    }

    randomButton.setImages(true, true, true,
        img, 1.0f, juce::Colours::transparentBlack,
        img, 1.0f, juce::Colours::transparentBlack,
        img2, 1.0f, juce::Colours::transparentBlack, //pas le pause
        0.5f );
    resized();
}


void PlayerComponent::nextButtonClicked(void)
{
    Master_changeState(Master_Stopping);  
    int nb = jsonParserGetNbSong();  
    int a;
    if(!randomMode)
    {

        if (currentIdxPlaying == (nb - 1)) //dernier son joué, retour au premier
        {
            currentIdxPlaying = 0;
        }
        else
        {
            currentIdxPlaying++;
        }
    }
    else
    {
        do
        {
            a = rand() % nb;
        } while (a == currentIdxPlaying);
        currentIdxPlaying = a;
    }

    Master_loadAndPlay(currentIdxPlaying);
    playerCallback(currentIdxPlaying);
}

void PlayerComponent::prevButtonClicked(void)
{
    Master_changeState(Master_Stopping);    
    if (currentIdxPlaying == 0) //dernier son joué, retour au premier
    {
        currentIdxPlaying = jsonParserGetNbSong() - 1;
        Master_loadAndPlay(currentIdxPlaying);
    }
    else
    {
        Master_loadAndPlay(--currentIdxPlaying);
    }
    
    playerCallback(currentIdxPlaying);
}


static std::unique_ptr<ScopedDPIAwarenessDisabler> makeDPIAwarenessDisablerForPlugin (const PluginDescription& desc)
{
    //pas de resize de plugin
    return nullptr;
}



void PlayerComponent::addPluginCallback(std::unique_ptr<AudioPluginInstance> instance, const String& error, int ii) //ii player à connecter
{
    if (instance == nullptr)
    {
        AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, TRANS("Error loading pluging"), error);
    }
    else
    {
        instance->enableAllBuses();
        auto VR3Node = mainProcessor->addNode(std::move(instance));
        for (int channel = 0; channel < 1; ++channel)     //connect_audio_node input/output
        {
            mainProcessor->addConnection({ { slots.getUnchecked(ii)->nodeID,  channel }, { VR3Node->nodeID, channel } });
        }
        for (int channel = 0; channel < 2; ++channel)     //connect_audio_node input/output
        {
            mainProcessor->addConnection({ { VR3Node->nodeID,  channel }, { audioOutputNode->nodeID, channel } });
        }

    }

    //init parameters
    AudioProcessorGraph::Node* node;
    node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID)(ii + MAX_INSTR + 2)); //vst3 dearvr pro
    if (node != nullptr)
    {
        if (auto* processor = node->getProcessor())
        {
            if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
            {
                if (auto* param = plugin->getParameters()[0]) //0 Polar system
                {
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

    slots.set(0, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(10))); //0 : id du player
    slots.set(1, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(11))); //0 : id du player
    slots.set(2, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(12))); //0 : id du player
    slots.set(3, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(13))); //0 : id du player    
    slots.set(4, mainProcessor->addNode ( std::make_unique<CustomPlayerProcessor>(13))); //0 : id du player    
    for(int ii = 0; ii< MAX_INSTR; ii++)
    {
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
            else
            {
                AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,TRANS("Error"),"12");     
            }
        }
    }


    vstformatManager1.addDefaultFormats();
    vstformatManager2.addDefaultFormats();
    vstformatManager3.addDefaultFormats();
    vstformatManager4.addDefaultFormats();
    vstformatManager5.addDefaultFormats();

    OwnedArray<PluginDescription> pluginDescriptions;    //https://forum.juce.com/t/connect-to-a-vst-plugin/20929/6
    KnownPluginList plist;
    for (int i = 0; i < vstformatManager1.getNumFormats(); ++i)
    {
        plist.scanAndAddFile(PathGetAsset(ASSET_VST), true, pluginDescriptions,  *vstformatManager1.getFormat(i));
    }
    jassert (pluginDescriptions.size() > 0);
    std::shared_ptr<ScopedDPIAwarenessDisabler> dpiDisabler = makeDPIAwarenessDisablerForPlugin(*pluginDescriptions[0]);

    vstformatManager1.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error, 0);
                                                                }
                                                            );
    vstformatManager2.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error,1);
                                                                }
                                                            );
    vstformatManager3.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error, 2);
                                                                }
                                                            );
    vstformatManager4.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error,3);
                                                                }
                                                            );
    vstformatManager5.createPluginInstanceAsync(         *pluginDescriptions[0],
                                                            48000,
                                                            1024,
                                                                [this, dpiDisabler](std::unique_ptr<AudioPluginInstance> instance, const String& error)
                                                                {
                                                                    addPluginCallback(std::move(instance), error,4);
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