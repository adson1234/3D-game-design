#include "TextRenderer.h"

template<> TextRenderer* Ogre::Singleton<TextRenderer>::ms_Singleton = 0;

TextRenderer::TextRenderer()
{
    _overlayMgr = Ogre::OverlayManager::getSingletonPtr();
	
    _overlay = _overlayMgr->create("overlay1");
    _panel = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container1"));
    _panel->setDimensions(1, 1);
    _panel->setPosition(0, 0);

    _overlay->add2D(_panel);

    _overlay->show();
}

void TextRenderer::addTextBox(const std::string& ID,
                const std::string& text,
                Ogre::Real x, Ogre::Real y,
                Ogre::Real width, Ogre::Real height,
                const Ogre::ColourValue& color)
{
	std::cout <<"test3"<<std::endl;
    Ogre::OverlayElement* textBox = _overlayMgr->createOverlayElement("TextArea", ID);
	std::cout <<"test4"<<std::endl;
    textBox->setDimensions(width, height);
    textBox->setMetricsMode(Ogre::GMM_PIXELS);
    textBox->setPosition(x, y);
    textBox->setWidth(width);
    textBox->setHeight(height);
    textBox->setParameter("font_name", "MyFont");
    textBox->setParameter("char_height", "0.3");
    textBox->setColour(color);
	std::cout <<"test5"<<std::endl;
    textBox->setCaption(text);
	std::cout <<"test6"<<std::endl;
    _panel->addChild(textBox);
	std::cout <<"test7"<<std::endl;
}

void TextRenderer::removeTextBox(const std::string& ID)
{
    _panel->removeChild(ID);
    _overlayMgr->destroyOverlayElement(ID);
}

void TextRenderer::setText(const std::string& ID, const std::string& Text)
{
    Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
    textBox->setCaption(Text);
}

const std::string& TextRenderer::getText(const std::string& ID)
{
    Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
    return textBox->getCaption();
}