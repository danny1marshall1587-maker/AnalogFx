#include "PluginProcessor.h"
#include "PluginEditor.h"

AnalogFxAudioProcessorEditor::AnalogFxAudioProcessorEditor(AnalogFxAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{


    bgTelefunken = juce::ImageCache::getFromMemory(BinaryData::telefunken_bg_png, BinaryData::telefunken_bg_pngSize);
    bgNevePre = juce::ImageCache::getFromMemory(BinaryData::neve_bg_png, BinaryData::neve_bg_pngSize);
    bgModernPre = juce::ImageCache::getFromMemory(BinaryData::modern_pre_bg_png, BinaryData::modern_pre_bg_pngSize);
    bgPultec = juce::ImageCache::getFromMemory(BinaryData::pultec_bg_png, BinaryData::pultec_bg_pngSize);
    bgModEq = juce::ImageCache::getFromMemory(BinaryData::modern_eq_bg_png, BinaryData::modern_eq_bg_pngSize);
    bgModComp = juce::ImageCache::getFromMemory(BinaryData::modern_comp_bg_png, BinaryData::modern_comp_bg_pngSize);
    bgNC76 = juce::ImageCache::getFromMemory(BinaryData::nc76_bg_png, BinaryData::nc76_bg_pngSize);
    bgLA2A = juce::ImageCache::getFromMemory(BinaryData::la2a_bg_png, BinaryData::la2a_bg_pngSize);
    bgFairchild = juce::ImageCache::getFromMemory(BinaryData::fairchild_bg_png, BinaryData::fairchild_bg_pngSize);
    bgDirtEq = juce::ImageCache::getFromMemory(BinaryData::dirt_eq_bg_png, BinaryData::dirt_eq_bg_pngSize);
    bgOutput = juce::ImageCache::getFromMemory(BinaryData::output_bg_png, BinaryData::output_bg_pngSize);

    addAndMakeVisible(preampSelector);
    preampSelector.addItemList({"Bypass", "Telefunken", "Neve", "Modern"}, 1);
    preampSelectorAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "preamp_type", preampSelector);
    preampSelector.addListener(this);

    addAndMakeVisible(compSelector);
    compSelector.addItemList({"Bypass", "NC76", "LA-2A", "Fairchild", "Modern VCA"}, 1);
    compSelectorAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "comp_type", compSelector);
    compSelector.addListener(this);

    addAndMakeVisible(eqSelector);
    eqSelector.addItemList({"Bypass", "Dirt EQ", "Neve 1073", "Pultec EQP-1A", "Modern Surgical"}, 1);
    eqSelectorAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "eq_type", eqSelector);
    eqSelector.addListener(this);

    addAndMakeVisible(scaleSelector);
    scaleSelector.addItemList({"Scale: 50%", "Scale: 75%", "Scale: 100%", "Scale: 150%", "Scale: 200%"}, 1);
    scaleSelector.setSelectedItemIndex(2, juce::dontSendNotification);
    scaleSelector.addListener(this);

    addAndMakeVisible(outputSelector);
    outputSelector.addItemList({"Bypass", "Vintage Tape", "British Iron", "Valve Summing", "Modern Polish"}, 1);
    outputSelectorAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "output_type", outputSelector);
    outputSelector.addListener(this);

    addAndMakeVisible(oversamplingButton);
    oversamplingButton.setLookAndFeel(&switchLaf);
    oversamplingAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "oversampling", oversamplingButton);

    addAndMakeVisible(oversamplingLabel);
    oversamplingLabel.setText("OVERSAMPLING", juce::dontSendNotification);
    oversamplingLabel.setFont(10.0f);
    oversamplingLabel.setJustificationType(juce::Justification::centred);
    oversamplingLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.7f));

    // Build Preamp Knobs (0)
    buildKnob("preamp_drive", "Drive", 0);
    buildKnob("preamp_trim", "Trim", 0);

    // Build Comp Knobs (1)
    buildKnob("comp_trim", "Trim", 1);
    buildKnob("nc76_thresh", "Threshold", 1);
    buildKnob("nc76_attack", "Attack", 1);
    buildKnob("nc76_release", "Release", 1);
    buildKnob("nc76_in_gain", "In Gain", 1);
    buildKnob("nc76_out_gain", "Out Gain", 1);
    buildKnob("nc76_mix", "Mix", 1);
    
    buildKnob("la2a_peak", "Peak", 1);
    buildKnob("la2a_gain", "Gain", 1);
    
    buildKnob("fc_l_thresh", "L Thresh", 1);
    buildKnob("fc_r_thresh", "R Thresh", 1);
    buildKnob("fc_l_bias", "L Bias", 1);
    buildKnob("fc_r_bias", "R Bias", 1);
    buildKnob("fc_l_makeup", "L Gain", 1);
    buildKnob("fc_r_makeup", "R Gain", 1);
    
    buildKnob("mc_thresh", "Threshold", 1);
    buildKnob("mc_ratio", "Ratio", 1);
    buildKnob("mc_attack", "Attack", 1);
    buildKnob("mc_release", "Release", 1);
    buildKnob("mc_makeup", "Makeup", 1);

    // Build EQ Knobs (2)
    buildKnob("eq_trim", "Trim", 2);
    
    buildKnob("lowpass_freq", "LPF", 2);
    buildKnob("lowpass_q", "LPF Q", 2);
    buildKnob("band1_gain", "HF Gain", 2);
    buildKnob("band1_freq", "HF Freq", 2);
    buildKnob("band2_gain", "HMF Gain", 2);
    buildKnob("band2_freq", "HMF Freq", 2);
    buildKnob("band2_q", "HMF Q", 2);
    buildKnob("band3_gain", "LMF Gain", 2);
    buildKnob("band3_freq", "LMF Freq", 2);
    buildKnob("band3_q", "LMF Q", 2);
    buildKnob("band4_gain", "LF Gain", 2);
    buildKnob("band4_freq", "LF Freq", 2);
    buildKnob("highpass_freq", "HPF", 2);
    buildKnob("highpass_q", "HPF Q", 2);
    
    buildKnob("nv_gain", "Gain", 2);
    buildKnob("nv_hf_gain", "HF", 2);
    buildKnob("nv_mf_gain", "MF", 2);
    buildKnob("nv_lf_gain", "LF", 2);
    buildKnob("nv_drive", "Drive", 2);
    
    buildKnob("pu_l_boost", "L Boost", 2);
    buildKnob("pu_l_atten", "L Atten", 2);
    buildKnob("pu_peak_q", "Peak Q", 2);
    buildKnob("pu_p_boost", "P Boost", 2);
    buildKnob("pu_h_atten", "H Atten", 2);

    buildButton("nv_phase", "PHASE", 2);
    buildButton("nv_hf_on", "HF ON", 2);
    buildButton("nv_mf_on", "MF ON", 2);
    buildButton("nv_lf_on", "LF ON", 2);
    buildButton("nv_hpf_on", "HPF ON", 2);
    buildButton("nv_eq_on", "MASTER", 2);
    
    buildKnob("ic1_drive", "IC1 DRV", 2);
    buildKnob("ic2_drive", "IC2 DRV", 2);
    buildKnob("drive_db", "DRIVE", 2);
    buildKnob("gain_db", "GAIN", 2);
    buildButton("dirt_eq_on", "EQ ON", 2);
    
    buildKnob("mq_l_freq", "LF Hz", 2);
    buildKnob("mq_l_gain", "LF Gain", 2);
    buildKnob("mq_m1_freq", "LMF Hz", 2);
    buildKnob("mq_m1_gain", "LMF Gain", 2);
    buildKnob("mq_m1_q", "LMF Q", 2);
    buildKnob("mq_m2_freq", "HMF Hz", 2);
    buildKnob("mq_m2_gain", "HMF Gain", 2);
    buildKnob("mq_m2_q", "HMF Q", 2);
    buildKnob("mq_h_freq", "HF Hz", 2);
    buildKnob("mq_h_gain", "HF Gain", 2);

    // Build Output Knobs (3)
    buildKnob("output_drive", "Drive", 3);
    buildButton("output_safe", "SAFE", 3);

    // Missing Dirt EQ Knobs
    buildKnob("lowpass_freq", "LP Hz", 2);
    buildKnob("lowpass_q", "LP Q", 2);
    buildKnob("band1_freq", "H Freq", 2);
    buildKnob("band1_gain", "High Gain", 2);
    buildKnob("band2_freq", "HM Freq", 2);
    buildKnob("band2_gain", "HM Gain", 2);
    buildKnob("band2_q", "HM Q", 2);
    buildKnob("band3_freq", "LM Freq", 2);
    buildKnob("band3_gain", "LM Gain", 2);
    buildKnob("band3_q", "LM Q", 2);
    buildKnob("band4_freq", "L Freq", 2);
    buildKnob("band4_gain", "Low Gain", 2);
    buildKnob("highpass_freq", "HP Hz", 2);
    buildKnob("highpass_q", "HP Q", 2);

    updateVisibility();
    setOpaque(true);
    openGLContext.setComponentPaintingEnabled(true);
    openGLContext.attachTo(*this);
    startTimerHz(30);
}

AnalogFxAudioProcessorEditor::~AnalogFxAudioProcessorEditor()
{
    openGLContext.detach();
    stopTimer();
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void AnalogFxAudioProcessorEditor::timerCallback()
{
    float preRMS = audioProcessor.preampLevel.load();
    float compRMS = audioProcessor.compLevel.load();
    float eqRMS = audioProcessor.eqLevel.load();
    
    preMeterVal = juce::jmax(preRMS, preMeterVal * 0.8f);
    compMeterVal = juce::jmax(compRMS, compMeterVal * 0.8f);
    eqMeterVal = juce::jmax(eqRMS, eqMeterVal * 0.8f);
    compGRVal = audioProcessor.compGR.load();
    
    repaint();
}

void AnalogFxAudioProcessorEditor::buildKnob(const juce::String& paramId, const juce::String& name, int section)
{
    auto slider = std::make_unique<juce::Slider>(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow);
    slider->setComponentID(paramId);
    
    if (paramId.startsWith("nv_")) slider->setLookAndFeel(&neveLaf);
    else if (paramId.startsWith("pu_")) slider->setLookAndFeel(&pultecLaf);
    else if (paramId.startsWith("nc76_")) slider->setLookAndFeel(&nc76Laf);
    else if (paramId.startsWith("fc_")) slider->setLookAndFeel(&fairchildLaf);
    else if (paramId.startsWith("la2a_")) slider->setLookAndFeel(&la2aLaf);
    else if (paramId.startsWith("mc_")) slider->setLookAndFeel(&modCompLaf);
    else if (paramId.startsWith("mq_")) slider->setLookAndFeel(&modEqLaf);
    else if (paramId.startsWith("band") || paramId.startsWith("lowpass") || paramId.startsWith("highpass") || paramId.startsWith("ic") || paramId == "drive_db" || paramId == "gain_db") slider->setLookAndFeel(&dirtEqLaf);
    else slider->setLookAndFeel(&modPreLaf); // Default for shared parameters until updateVisibility takes over
    
    if (paramId.containsIgnoreCase("gain") || paramId.containsIgnoreCase("drive") || paramId.containsIgnoreCase("db") || paramId.containsIgnoreCase("atten") || paramId.containsIgnoreCase("thresh"))
        slider->setTextValueSuffix(" dB");
    else if (paramId.containsIgnoreCase("freq") || paramId.containsIgnoreCase("hz") || paramId.containsIgnoreCase("pass"))
        slider->setTextValueSuffix(" Hz");

    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 16);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(*slider);

    auto label = std::make_unique<juce::Label>("", name);
    label->setJustificationType(juce::Justification::centred);
    label->setColour(juce::Label::textColourId, juce::Colours::white);
    label->setFont(12.0f);
    addAndMakeVisible(*label);

    if (section == 0) {
        preAtts.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramId, *slider));
        preSliders.push_back(std::move(slider));
        preLabels.push_back(std::move(label));
    } else if (section == 1) {
        compAtts.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramId, *slider));
        compSliders.push_back(std::move(slider));
        compLabels.push_back(std::move(label));
    } else if (section == 2) {
        eqAtts.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramId, *slider));
        eqSliders.push_back(std::move(slider));
        eqLabels.push_back(std::move(label));
    } else if (section == 3) {
        outputAtts.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramId, *slider));
        outputSliders.push_back(std::move(slider));
        outputLabels.push_back(std::move(label));
    }
}

void AnalogFxAudioProcessorEditor::buildButton(const juce::String& paramId, const juce::String& name, int section)
{
    auto btn = std::make_unique<juce::ToggleButton>(name);
    btn->setComponentID(paramId);
    btn->setLookAndFeel(&modernButtonLaf);
    addAndMakeVisible(*btn);
    
    auto att = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, paramId, *btn);
    
    if (section == 2) {
        eqButtonAtts.push_back(std::move(att));
        eqButtons.push_back(std::move(btn));
    } else if (section == 3) {
        outputButtonAtts.push_back(std::move(att));
        outputButtons.push_back(std::move(btn));
    }
}

void AnalogFxAudioProcessorEditor::comboBoxChanged(juce::ComboBox* cb)
{
    if (cb == &scaleSelector)
    {
        int idx = scaleSelector.getSelectedItemIndex();
        if (idx == 0) currentScale = 0.5f;
        else if (idx == 1) currentScale = 0.75f;
        else if (idx == 2) currentScale = 1.0f;
        else if (idx == 3) currentScale = 1.5f;
        else if (idx == 4) currentScale = 2.0f;
    }
    
    updateVisibility();
}

void AnalogFxAudioProcessorEditor::updateVisibility()
{
    int preType = preampSelector.getSelectedId();
    int compType = compSelector.getSelectedId();
    int eqType = eqSelector.getSelectedId();

    int baseW = 800;
    int baseH = (eqType == 2) ? 1500 : 1200;

    int targetW = (int)(baseW * currentScale);
    int targetH = (int)(baseH * currentScale);

    for (auto& s : preSliders) s->setVisible(false);
    for (auto& l : preLabels) l->setVisible(false);
    for (auto& s : compSliders) s->setVisible(false);
    for (auto& l : compLabels) l->setVisible(false);
    for (auto& s : eqSliders) s->setVisible(false);
    for (auto& l : eqLabels) l->setVisible(false);
    for (auto& b : eqButtons) b->setVisible(false);

    auto updateLaf = [&](const juce::String& param, juce::LookAndFeel* laf) {
        auto check = [&](auto& list) {
            for (auto& s : list) {
                if (s->getComponentID() == param) {
                    if (&s->getLookAndFeel() != laf) s->setLookAndFeel(laf);
                }
            }
        };
        check(preSliders);
        check(compSliders);
        check(eqSliders);
        check(outputSliders);
    };

    if (preType == 2) { updateLaf("preamp_drive", &telefunkenLaf); updateLaf("preamp_trim", &telefunkenLaf); }
    else if (preType == 3) { updateLaf("preamp_drive", &neveLaf); updateLaf("preamp_trim", &neveLaf); }
    else if (preType == 4) { updateLaf("preamp_drive", &modPreLaf); updateLaf("preamp_trim", &modPreLaf); }

    if (compType == 2) updateLaf("comp_trim", &nc76Laf);
    else if (compType == 3) updateLaf("comp_trim", &la2aLaf);
    else if (compType == 4) updateLaf("comp_trim", &fairchildLaf);
    else if (compType == 5) updateLaf("comp_trim", &modCompLaf);

    if (eqType == 2) updateLaf("eq_trim", &dirtEqLaf);
    else if (eqType == 3) updateLaf("eq_trim", &neveLaf);
    else if (eqType == 4) updateLaf("eq_trim", &pultecLaf);
    else if (eqType == 5) updateLaf("eq_trim", &modEqLaf);

    updateLaf("output_drive", &outputLaf);

    // Preamp Visibility
    if (preType > 1) { // 1 is Bypass
        for (size_t i = 0; i < preSliders.size(); ++i) {
            preSliders[i]->setVisible(true);
            preLabels[i]->setVisible(true);
        }
    }

    // Comp Visibility
    if (compType > 1) {
        juce::String prefix = "";
        if (compType == 2) prefix = "nc76";
        else if (compType == 3) prefix = "la2a";
        else if (compType == 4) prefix = "fc";
        else if (compType == 5) prefix = "mc";

        for (size_t i = 0; i < compSliders.size(); ++i) {
            if (compSliders[i]->getComponentID().startsWith(prefix) || compSliders[i]->getComponentID() == "comp_trim") {
                compSliders[i]->setVisible(true);
                compLabels[i]->setVisible(true);
            }
        }
    }

    // EQ Visibility
    if (eqType > 1) {
        juce::String prefix = "";
        if (eqType == 2) prefix = "band"; // Dirt uses band, highpass, etc.
        else if (eqType == 3) prefix = "nv";
        else if (eqType == 4) prefix = "pu";
        else if (eqType == 5) prefix = "mq";

        for (size_t i = 0; i < eqSliders.size(); ++i) {
            auto id = eqSliders[i]->getComponentID();
            if (id.startsWith(prefix) || id == "eq_trim" || 
               (eqType == 2 && (id.startsWith("lowpass") || id.startsWith("highpass") || id.startsWith("ic") || id == "drive_db" || id == "gain_db"))) {
                eqSliders[i]->setVisible(true);
                eqLabels[i]->setVisible(true);
            }
        }
        
        for (auto& b : eqButtons) {
            auto id = b->getComponentID();
            if (id.startsWith(prefix) || (eqType == 2 && id == "dirt_eq_on")) b->setVisible(true);
        }
    }

    // Output Visibility
    int outType = outputSelector.getSelectedId();
    for (auto& s : outputSliders) s->setVisible(false);
    for (auto& l : outputLabels) l->setVisible(false);
    if (outType > 1) {
        for (size_t i = 0; i < outputSliders.size(); ++i) {
            outputSliders[i]->setVisible(true);
            outputLabels[i]->setVisible(true);
        }
        for (auto& b : outputButtons) b->setVisible(true);
    }

    // Now that visibility is set, handle layout and possible resize
    if (getWidth() != targetW || getHeight() != targetH)
    {
        setSize(targetW, targetH);
    }
    else
    {
        resized();
    }
    
    repaint();
    openGLContext.triggerRepaint();
}

void AnalogFxAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff111111));

    int w = getWidth();
    
    int preType = preampSelector.getSelectedId();
    int compType = compSelector.getSelectedId();
    int eqType = eqSelector.getSelectedId();
    int outType = outputSelector.getSelectedId();

    int preH = (int)(300 * currentScale);
    int compH = (int)(300 * currentScale);
    int eqH = (eqType == 2) ? (int)(600 * currentScale) : (int)(300 * currentScale);
    int outH = (int)(300 * currentScale);

    int preY = 0;
    int compY = preH;
    int eqY = compY + compH;
    int outY = eqY + eqH;

    // Draw Preamp BG
    if (preType == 2 && s_bgTelefunken.isValid()) g.drawImageAt(s_bgTelefunken, 0, preY);
    else if (preType == 3 && s_bgNevePre.isValid()) g.drawImageAt(s_bgNevePre, 0, preY);
    else if (preType == 4 && s_bgModernPre.isValid()) g.drawImageAt(s_bgModernPre, 0, preY);
    
    // Draw Comp BG
    if (compType == 2 && s_bgNC76.isValid()) g.drawImageAt(s_bgNC76, 0, compY);
    else if (compType == 3 && s_bgLA2A.isValid()) g.drawImageAt(s_bgLA2A, 0, compY);
    else if (compType == 4 && s_bgFairchild.isValid()) g.drawImageAt(s_bgFairchild, 0, compY);
    else if (compType == 5 && s_bgModComp.isValid()) g.drawImageAt(s_bgModComp, 0, compY);
 
    // Draw EQ BG
    if (eqType == 2 && s_bgDirtEq.isValid()) g.drawImageAt(s_bgDirtEq, 0, eqY);
    else if (eqType == 3 && s_bgNevePre.isValid()) g.drawImageAt(s_bgNevePre, 0, eqY);
    else if (eqType == 4 && s_bgPultec.isValid()) g.drawImageAt(s_bgPultec, 0, eqY);
    else if (eqType == 5 && s_bgModEq.isValid()) g.drawImageAt(s_bgModEq, 0, eqY);
 
    // Draw Output BG
    if (outType > 1 && s_bgOutput.isValid()) g.drawImageAt(s_bgOutput, 0, outY);

    // Draw LED Meters
    auto drawMeter = [&](int sectionIndex, float meterValue, juce::Colour color, bool isGR = false) {
        int curSecY = (sectionIndex == 0) ? preY : (sectionIndex == 1) ? compY : (sectionIndex == 2) ? eqY : outY;
        int meterW = (int)(isGR ? 8 : 12) * currentScale;
        int meterH = (int)(200 * currentScale);
        int x = w - (int)(isGR ? 65 : 40) * currentScale;
        int y = curSecY + (int)(50 * currentScale);

        g.setColour(juce::Colours::black.withAlpha(0.7f));
        g.fillRoundedRectangle(x, y, meterW, meterH, 2.0f);

        float db = isGR ? meterValue : (20.0f * std::log10(std::max(meterValue, 0.0001f)));
        float minDb = isGR ? 0.0f : -60.0f;
        float maxDb = isGR ? 20.0f : 6.0f;
        float norm = juce::jlimit(0.0f, 1.0f, (db - minDb) / (maxDb - minDb));
        
        int fillH = static_cast<int>(norm * meterH);
        
        juce::Colour finalColor = color;
        if (isGR) {
            if (meterValue > 6.0f) finalColor = juce::Colours::red;
            else finalColor = juce::Colour(0xffffd700); // Gold
        } else {
            if (db > -6.0f) finalColor = juce::Colours::red;
            else if (db > -18.0f) finalColor = juce::Colours::orange;
        }

        g.setColour(finalColor);
        if (isGR) g.fillRect(x, y, meterW, fillH);
        else g.fillRect(x, y + meterH - fillH, meterW, fillH);
        
        // Scale Markers
        g.setColour(juce::Colours::white.withAlpha(0.4f));
        g.setFont(8.0f * currentScale);
        if (isGR) {
            for (int mark : {0, 3, 6, 9, 12, 18}) {
                float mY = y + (mark / 20.0f) * meterH;
                g.drawHorizontalLine((int)mY, (float)x - 3, (float)x);
                if (mark % 6 == 0) g.drawText(juce::String(mark), x - (int)(15 * currentScale), (int)mY - 5, (int)(12 * currentScale), 10, juce::Justification::right);
            }
        } else {
            for (int mark : {0, -6, -18, -36, -60}) {
                float mNorm = (mark + 60.0f) / 66.0f;
                float mY = y + (1.0f - mNorm) * meterH;
                g.drawHorizontalLine((int)mY, (float)x - 3, (float)x);
                if (mark == 0 || mark == -18 || mark == -60)
                    g.drawText(juce::String(mark), x - (int)(22 * currentScale), (int)mY - 5, (int)(18 * currentScale), 10, juce::Justification::right);
            }
        }
    };
    
    if (preType > 1) drawMeter(0, preMeterVal, juce::Colours::white);
    if (compType > 1) {
        drawMeter(1, compMeterVal, juce::Colours::orange);
        drawMeter(1, std::abs(compGRVal), juce::Colour(0xffffd700), true);
    }
    if (eqType > 1) drawMeter(2, eqMeterVal, juce::Colours::cyan);
    if (outType > 1) drawMeter(3, eqMeterVal, juce::Colours::limegreen); // Reuse eqMeterVal for output for now
}

void AnalogFxAudioProcessorEditor::resized()
{
    int w = getWidth();
    int margin = (int)(20 * currentScale);
    auto selectorW = (int)(150 * currentScale);
    auto selectorH = (int)(24 * currentScale);

    // Variable Section Heights
    int preH = (int)(300 * currentScale);
    int compH = (int)(300 * currentScale);
    int eqH = (eqSelector.getSelectedId() == 2) ? (int)(600 * currentScale) : (int)(300 * currentScale);
    int outH = (int)(300 * currentScale);

    int preY = 0;
    int compY = preH;
    int eqY = compY + compH;
    int outY = eqY + eqH;

    // GPU Optimization: Pre-scale backgrounds to match the window size once
    auto scaleBg = [&](const juce::Image& src, juce::Image& dest, int tw, int th) {
        if (src.isValid()) dest = src.rescaled(tw, th, juce::Graphics::highResamplingQuality);
    };

    scaleBg(bgTelefunken, s_bgTelefunken, w, preH);
    scaleBg(bgNevePre, s_bgNevePre, w, preH);
    scaleBg(bgModernPre, s_bgModernPre, w, preH);
    scaleBg(bgNC76, s_bgNC76, w, compH);
    scaleBg(bgLA2A, s_bgLA2A, w, compH);
    scaleBg(bgFairchild, s_bgFairchild, w, compH);
    scaleBg(bgModComp, s_bgModComp, w, compH);
    scaleBg(bgDirtEq, s_bgDirtEq, w, eqH);
    scaleBg(bgPultec, s_bgPultec, w, eqH);
    scaleBg(bgModEq, s_bgModEq, w, eqH);
    scaleBg(bgOutput, s_bgOutput, w, outH);

    preampSelector.setBounds(margin, preY + (int)(20 * currentScale), selectorW, selectorH);
    compSelector.setBounds(margin, compY + (int)(20 * currentScale), selectorW, selectorH);
    eqSelector.setBounds(margin, eqY + (int)(20 * currentScale), selectorW, selectorH);
    outputSelector.setBounds(margin, outY + (int)(20 * currentScale), selectorW, selectorH);
    
    scaleSelector.setBounds(w - selectorW - margin, preY + (int)(20 * currentScale), selectorW, selectorH);
    
    // Oversampling Button (Top section, away from meter)
    int osW = (int)(140 * currentScale);
    int osH = (int)(45 * currentScale);
    int osX = w - osW - (int)(150 * currentScale); 
    int osY = preY + (int)(20 * currentScale) + selectorH + margin;
    oversamplingButton.setBounds(osX, osY, osW, osH);
    oversamplingLabel.setBounds(osX, osY + osH, osW, (int)(15 * currentScale));

    // Safe Button Layout (Output section, away from meter)
    int safeW = (int)(120 * currentScale);
    int safeH = (int)(40 * currentScale);
    int safeX = w - safeW - (int)(150 * currentScale);
    int safeY = outY + (int)(20 * currentScale) + selectorH + margin;
    for (auto& b : outputButtons) {
        b->setBounds(safeX, safeY, safeW, safeH);
    }

    auto getKnobSize = [&](int section, const juce::String& id) {
        if (section == 0) return (int)(130 * currentScale); // Preamp
        if (section == 2) { // EQ
            if (id.containsIgnoreCase("freq") || id.containsIgnoreCase("q") || id.endsWithIgnoreCase("hz")) 
                return (int)(60 * currentScale); // Smaller for EQ
            return (int)(85 * currentScale); 
        }
        if (section == 3) return (int)(130 * currentScale); // Output
        return (int)(80 * currentScale); 
    };

    auto layoutSection = [&](std::vector<std::unique_ptr<juce::Slider>>& sliders, std::vector<std::unique_ptr<juce::Label>>& labels, int startY, int sectionIdx) {
        std::vector<int> vis;
        for (size_t i = 0; i < sliders.size(); ++i) {
            if (sliders[i]->isVisible()) vis.push_back((int)i);
        }
        if (vis.empty()) return;

        struct Row { std::vector<int> idxs; int width = 0; int maxH = 0; };
        std::vector<Row> rows;
        Row cur;
        int maxW = w - (int)(100 * currentScale); // Space for meter
        int spacingX = (int)(20 * currentScale);

        for (int i : vis) {
            int sz = getKnobSize(sectionIdx, sliders[i]->getComponentID());
            if (!cur.idxs.empty() && cur.width + spacingX + sz > maxW) {
                rows.push_back(cur);
                cur = Row();
            }
            cur.idxs.push_back(i);
            cur.width += (cur.idxs.size() > 1 ? spacingX : 0) + sz;
            cur.maxH = std::max(cur.maxH, sz);
        }
        if (!cur.idxs.empty()) rows.push_back(cur);

        int curY = startY + (int)(60 * currentScale);
        for (auto& row : rows) {
            int x = (w - row.width) / 2;
            for (int i : row.idxs) {
                int sz = getKnobSize(sectionIdx, sliders[i]->getComponentID());
                int yOff = (row.maxH - sz) / 2;
                sliders[i]->setBounds(x, curY + yOff, sz, sz);
                labels[i]->setBounds(x - (int)(10 * currentScale), curY + yOff - (int)(20 * currentScale), sz + (int)(20 * currentScale), (int)(20 * currentScale));
                x += sz + spacingX;
            }
            curY += row.maxH + (int)(40 * currentScale);
        }
    };

    layoutSection(preSliders, preLabels, preY, 0);
    layoutSection(compSliders, compLabels, compY, 1);
    layoutSection(eqSliders, eqLabels, eqY, 2);
    layoutSection(outputSliders, outputLabels, outY, 3);
    
    // Layout EQ Buttons
    int btnX = margin + selectorW + margin;
    int btnY = eqY + (int)(20 * currentScale);
    for (auto& b : eqButtons) {
        if (b->isVisible()) {
            b->setBounds(btnX, btnY, (int)(80 * currentScale), selectorH);
            btnX += (int)(90 * currentScale);
            if (btnX > w - (int)(150 * currentScale)) {
                btnX = margin + selectorW + margin;
                btnY += selectorH + (int)(5 * currentScale);
            }
        }
    }
}
