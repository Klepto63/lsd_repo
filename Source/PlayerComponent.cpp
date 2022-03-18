#include "PlayerComponent.h"


bool PlayerComponent::isPlaying(void)
{
    if (state == Playing)
    {
        return true;
    }
    return false;
}

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
        float v = musicSlider.getValue();
        audio1.setPosition(v * audio1.getLengthInSeconds() / 10);
        //audio2.setPosition(v * audio2.getLengthInSeconds() / 10);
        //audio3.setPosition(v * audio3.getLengthInSeconds() / 10);
        //audio4.setPosition(v * audio4.getLengthInSeconds() / 10);        
        musicSliderBlockTimer = false;
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

    if (slider == &energySlider)//1..10
    {
        //glue reset
        if (abs(energySlider.getValue() - 5) < 0.31 )
        {
            energySlider.setValue(5, dontSendNotification);
        }
    }
    
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


void PlayerComponent::playCommand(void)
{

}

void PlayerComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerComponent::paint(juce::Graphics& g)
{
    g.fillAll(Colour(WP_PLAYER));
}

void PlayerComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    /*
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    mixerAudioSource.getNextAudioBlock(bufferToFill);
    */
    auto level = (float) volumeSlider.getValue()/8;
    //if (level == 0)
    //{
    //    return;
    //}
    mixerAudioSource.getNextAudioBlock(bufferToFill);
    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels  = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();
    auto maxInputChannels  = activeInputChannels .getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;
    for (auto channel = 0; channel < maxOutputChannels; ++channel)
        {
            if ((! activeOutputChannels[channel]) || maxInputChannels == 0)
            {
                bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else
            {
                auto actualInputChannel = channel % maxInputChannels; // [1]
                if (! activeInputChannels[channel]) // [2]
                {
                    bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
                }
                else // [3]
                {
                    auto* inBuffer = bufferToFill.buffer->getReadPointer (actualInputChannel,
                                                                          bufferToFill.startSample);
                    auto* outBuffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
                    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    {
                        outBuffer[sample] = inBuffer[sample] * level;
                    }
                }
            }
        }
}

void PlayerComponent::releaseResources()
{
    mixerAudioSource.releaseResources();
}

void PlayerComponent::resized()
{
    energySlider.setBounds((getWidth() - 100) * 0.5f - 1, 5, 100, 80);
    playButton.setBounds((getWidth() - PLAY_BUTTON_SIZE) * 0.5f , 45 - 0.5* PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE, PLAY_BUTTON_SIZE); //aussi resized a updatePlayerButtonImage()
    prevButton.setBounds((getWidth() - NEXT_BUTTON_SIZE) * 0.5f - NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE);
    nextButton.setBounds((getWidth() - NEXT_BUTTON_SIZE) * 0.5f + NEXT_BUTTON_SPACE, 45 - 0.5 * NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE, NEXT_BUTTON_SIZE);
    musicSlider.setBounds(0.5 * (getWidth() - MUSIC_SLIDER_SIZE), 58, MUSIC_SLIDER_SIZE, 42);
    lengthLabel.setBounds(0.5 * (getWidth() + MUSIC_SLIDER_SIZE), 59, getWidth() - 20, 40);
    currentPositionLabel.setBounds(0.5 * (getWidth() - MUSIC_SLIDER_SIZE)-40, 59, getWidth() - 20, 40);

    playerTitlePlayingComponent.setBounds(10, 10, 200, 80);
    volumeSlider.setBounds(getWidth() - 200, 45, 150, 20);
    muteButton.setBounds(getWidth() - 200 - MUTE_BUTTON_SIZE, 45, MUTE_BUTTON_SIZE, MUTE_BUTTON_SIZE); //aussi resized a updateMuteButtonImage()


    CBScenes.setBounds(185, 15,100,22);//22);

    DebugButton.setBounds(200,20,60,30);

}

void PlayerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    //if (source == &mixerAudioSource)
    //{
        if (audio1.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    //}

}

void PlayerComponent::timerCallback()
{
    if (audio1.isPlaying())
    {
        juce::RelativeTime position(audio1.getCurrentPosition());
        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto positionString = juce::String::formatted("%02d:%02d", minutes, seconds);
        currentPositionLabel.setText(positionString, juce::dontSendNotification);
        if (!musicSliderBlockTimer)
        {
            musicSlider.setValue(10 * audio1.getCurrentPosition() / audio1.getLengthInSeconds(), dontSendNotification);
        }
    }
    else
    {
        //currentPositionLabel.setText("Stopped", juce::dontSendNotification);
    }
}

void PlayerComponent::updateLoopState(bool shouldLoop)
{
    if (readerSource_audio1.get() != nullptr)
    {
        readerSource_audio1->setLooping(shouldLoop);
    }
	/*
    if (readerSource_audio2.get() != nullptr)
    {
        readerSource_audio2->setLooping(shouldLoop);
    }
    if (readerSource_audio3.get() != nullptr)
    {
        readerSource_audio3->setLooping(shouldLoop);
    }
    if (readerSource_audio4.get() != nullptr)
    {
        readerSource_audio4->setLooping(shouldLoop);
    } 
	*/       
}

void PlayerComponent::loadAndPlay(int idx)
{
    s_metadata md;
    if (!jsonParserLoad(idx, &md))
    {
        playerTitlePlayingComponent.loadSongData(md);
        currentIdxPlaying = idx; //todo optimiser
        auto* reader_audio1 = formatManager.createReaderFor(File(md.stem[0].path));
        //auto* reader_audio2 = formatManager.createReaderFor(File(md.stem[1].path));
        //auto* reader_audio3 = formatManager.createReaderFor(File(md.stem[2].path));
        //auto* reader_audio4 = formatManager.createReaderFor(File(md.stem[3].path));
        if (reader_audio1 != nullptr)
        {
            //std::unique_ptr<PositionableAudioSource> tmpSource(new AudioFormatReaderSource(reader, true));
            std::unique_ptr<juce::AudioFormatReaderSource> newSource1(new juce::AudioFormatReaderSource(reader_audio1, true));
            //std::unique_ptr<juce::AudioFormatReaderSource> newSource2(new juce::AudioFormatReaderSource(reader_audio2, true));  
            //std::unique_ptr<juce::AudioFormatReaderSource> newSource3(new juce::AudioFormatReaderSource(reader_audio3, true));  
            //std::unique_ptr<juce::AudioFormatReaderSource> newSource4(new juce::AudioFormatReaderSource(reader_audio4, true));  
            audio1.setSource(newSource1.get(), 0, nullptr, reader_audio1->sampleRate);
            //audio2.setSource(newSource2.get(), 0, nullptr, reader_audio2->sampleRate);
            //audio3.setSource(newSource3.get(), 0, nullptr, reader_audio3->sampleRate);
            //audio4.setSource(newSource4.get(), 0, nullptr, reader_audio4->sampleRate);
            playButton.setEnabled(true);
            energySlider.setEnabled(true);
            readerSource_audio1.reset(newSource1.release());
            //readerSource_audio2.reset(newSource2.release());
            //readerSource_audio3.reset(newSource3.release());
            //readerSource_audio4.reset(newSource4.release());

            changeState(Starting);
            CBScenes.setVisible(true);
        }
        else
        {
            int t = 0;
            t++; 
        }
    }
    else
    {
        //song not found
    }
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void PlayerComponent::changeState(TransportState newState)
{
    int lengthDuration_s;
    int minutes; 
    int seconds;
    String LengthString;
    if (state != newState)
    {
        state = newState;
        switch (state)
        {
        case Stopped:
            updatePlayerButtonImage(false);
            audio1.setPosition(0.0);
            audio2.setPosition(0.0);
            audio3.setPosition(0.0);
            audio4.setPosition(0.0);
            break;
        case Starting:
            updatePlayerButtonImage(true);

            audio1.start();
            //audio2.start();
            //audio3.start();
            //audio4.start();


            lengthDuration_s = audio1.getLengthInSeconds();
            minutes = ((int)(lengthDuration_s / 60));
            seconds = lengthDuration_s - (60 * minutes);
            LengthString = juce::String::formatted("%02d:%02d", minutes, seconds);
            lengthLabel.setText(LengthString, juce::dontSendNotification);
            musicSlider.setValue(0, dontSendNotification);
            break;
        case Playing:
            updatePlayerButtonImage(true);
            break;
        case Stopping:
            updatePlayerButtonImage(false);
            audio1.stop();
            //audio2.stop();
            //audio3.stop();
            //audio4.stop();            
            break;
        }
    }
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
    if (isMuted)
    {
        updateVolumeButtonImage(false, currentVolume);
        isMuted = false;
        currentVolume = volumeBeforeMute;;
        volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
    }
    else
    {
        updateVolumeButtonImage(true, currentVolume);
        isMuted = true;
        volumeBeforeMute = currentVolume;
        currentVolume = 0;
        volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
    }
}

void PlayerComponent::playButtonClicked()
{
    if (audio1.isPlaying())
    {
        changeState(Stopping);
    }
    else
    {
        changeState(Starting);
    }
}

void PlayerComponent::nextButtonClicked(void)
{
    if (currentIdxPlaying == jsonParserGetNbSong() - 1) //dernier son jou�, retour au premier
    {
        loadAndPlay(0);
        currentIdxPlaying = 0;
    }
    else
    {
        loadAndPlay(++currentIdxPlaying);
    }
}

void PlayerComponent::prevButtonClicked(void)
{
        if (currentIdxPlaying == 0) //dernier son joué, retour au premier
    {
        currentIdxPlaying = jsonParserGetNbSong() - 1;
        loadAndPlay(currentIdxPlaying);
    }
    else
    {
        loadAndPlay(--currentIdxPlaying);
    }
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

        //connect to graph
        //if (auto node = graph.addNode (std::move (instance)))
        //{
            //NODE MIDDLE


        //for (int channel = 0; channel < 1; ++channel)     //connect_audio_node input/output
        //{
        //    mainProcessor->addConnection({ { slots.getUnchecked(0)->nodeID,  channel }, { audioOutputNode->nodeID, channel } });
        //}

        
   
        auto VR3Node = mainProcessor->addNode(std::move(instance));
        for (int channel = 0; channel < 1; ++channel)     //connect_audio_node input/output
        {
            mainProcessor->addConnection({ { slots.getUnchecked(0)->nodeID,  channel }, { VR3Node->nodeID, channel } });
        }

        for (int channel = 0; channel < 2; ++channel)     //connect_audio_node input/output
        {
          //  mainProcessor->addConnection({ { VR3Node->nodeID,  channel }, { audioOutputNode->nodeID, channel } });
        }
        



            //node->properties.set ("x", pos.x);
            //node->properties.set ("y", pos.y);
            //changed();
        //}

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
    slots.set(0, mainProcessor->addNode(std::make_unique<OscillatorProcessor>()));

    auto slot = slots.getUnchecked(0); //useless mais pour tester
    if (slot != nullptr)
    {
        if (slot->getProcessor()->getName() == "Oscillator")
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