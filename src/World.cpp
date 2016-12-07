/*
 * Author: King Hoe Wong, Warren Schweigert
 * Desc: Implementation for World class.
 * Courtesy of: http://becomingindiedev.blogspot.com/2013/10/qt-5-and-sfml-20-integration.html
 *
 */

#include "World.h"

#include <QResizeEvent>
#include <QSize>
#include <QQueue>
#include <QString>
#include <SFML/Graphics.hpp>
#include "State.h"
#include "ResourceIdentifiers.h"
#include "ResourceManager.h"
#include "qdebug.h"

#include <iostream>

using namespace sf;
World::World(const QPoint &pos, const QSize &size, State::Context &context, QWidget *parent)
    : QSFMLWidget(pos, size, context, parent),mCharacterRelativePos(std::complex<int>(9,9))
{
                map.setRefSize(512);

                sf::VideoMode temp = sf::VideoMode::getDesktopMode();
                double height = temp.height;
                mScreenScale = height/1080;

                 leftPressed = false;
                 rightPressed = false;
                 upPressed = false;
                 downPressed = false;
                 mCharacterDirection = characterDirection::U;
}

void World::onInit()
{
    WorldLoader(2);

    getContext().textures.load(0, "qrc:/../media/Textures/meme.png");
    getContext().textures.get(0).setSmooth(true);
    getContext().textures.load(-1, "qrc:/../media/Textures/Fire.png");
    getContext().textures.get(-1).setSmooth(true);
    getContext().textures.load(-2, "qrc:/../media/Textures/Bridge.png");
    getContext().textures.get(-2).setSmooth(true);
    getContext().textures.load(-3, "qrc:/../media/Textures/Sand.png");
    getContext().textures.get(-3).setSmooth(true);
    getContext().textures.load(-4, "qrc:/../media/Textures/QuickSand.png");
    getContext().textures.get(-4).setSmooth(true);
}

void World::keyPressEvent(QKeyEvent* event)
{
    std::complex<int> temp = mWorldLocation;


    if(event->key() ==Qt::Key_Right){
        mCharacterDirection =characterDirection::R;
        temp +=std::complex<int>(1,0);
    }
    else if(event->key() ==Qt::Key_Left){
        mCharacterDirection =characterDirection::L;
        temp +=std::complex<int>(-1,0);
    }
    else if(event->key() ==Qt::Key_Up){
        mCharacterDirection =characterDirection::U;
        temp +=std::complex<int>(0,-1);
    }
    else if(event->key() ==Qt::Key_Down){
        mCharacterDirection =characterDirection::D;
        temp +=std::complex<int>(0,1);
    }

    if(moveValid(temp)){
        mWorldLocation = temp;
        if(plantAtSpot(mWorldLocation +mCharacterRelativePos))
        {
            emit plantPicked();
        }
    }
}

void World::onDraw(sf::RenderTarget& target, sf::RenderStates states)
{

    clear();
    target.clear();
    int jumpgap = 32*mScreenScale;
    for(int x = 0;x<576*mScreenScale;x+=jumpgap)
    {
        for(int y = 0;y<576*mScreenScale;y+=jumpgap)
        {
            //get land type  and display it
            std::complex<int> tempLocation = mWorldLocation + std::complex<int>(x/jumpgap,y/jumpgap);
            int type = getLandValue(tempLocation);

            mSprite.setTexture(mLandTextures[type]);
            mSprite.setPosition(x,y);
            mSprite.setScale(mScreenScale,mScreenScale);
            target.draw(mSprite, states);

            //if the main charector is here
            bool atCharectorX =  mCharacterRelativePos.real() == x/jumpgap;
            bool atCharectorY = mCharacterRelativePos.imag() == y/jumpgap;
            if(atCharectorX&&atCharectorY)
            {
                mCharacter.setTexture(getContext().textures.get(-static_cast<int>(mCharacterDirection)));
                mCharacter.setPosition(x,y-16);
                mCharacter.setScale(mScreenScale/2,mScreenScale/2);
                mCharacter.setOrigin(0,.25);
                target.draw(mCharacter,states);
            }

            if(plantAtSpot(tempLocation))
            {
                mCharacter.setTexture(getContext().textures.get(0));
                mCharacter.setPosition(x,y-16);
                mCharacter.setScale(mScreenScale/2,mScreenScale/2);
                mCharacter.setOrigin(0,.25);
                target.draw(mCharacter,states);
            }
        }
    }
}

void World::WorldLoader(int worldtype)
{
    mWorldNum = worldtype;
    if(worldtype==0)
    {
        //*
        mCurrentLandNameIndex = {"Bridge","DeepFreshWater","DeepSaltWater","Dirt","Fire","Grass","Mountain","QuickSand","RedSand",
                                "Sand","ShallowFreshWater", "ShallowSaltWater","Tree"};
        map.setJuliaValue(std::complex<double>(-.621,0));
        mUnmoveableTerrain = {1,2,4,6,7};
    }
    else if(worldtype==1)
    {
        map.setJuliaValue(std::complex<double>(-.5,.002));
        mCurrentLandNameIndex = {"Dirt","Grass","Mountain","Grass","Mountain","Sand","ShallowFreshWater","Tree","ShallowSaltWater","Tree"};
        mUnmoveableTerrain = {2,4};
    }
    else if(worldtype==2){
        map.setJuliaValue(std::complex<double>(-.5,-.002));
        mCurrentLandNameIndex = {"Dirt","Grass","Dirt","Mountain","QuickSand","RedSand","Sand","QuickSand","RedSand","Sand","ShallowFreshWater","Tree"};
        mUnmoveableTerrain = {4,7};
    }
    else {



    }

    mLandTextures.clear();
    for(int i = 1; i < mCurrentLandNameIndex.size()+1;i++)
    {
        QString tempstr = "qrc:/../media/Textures/"+mCurrentLandNameIndex[i-1]+".png";
        std::cout<<tempstr.toStdString()<<std::endl;
        getContext().textures.load(i, tempstr.toStdString());

        mLandTextures.push_back(getContext().textures.get(i));
    }
    mLandCount = mCurrentLandNameIndex.size();
}

bool World::moveValid(std::complex<int> next)
{
    next = next + mCharacterRelativePos;
    std::complex<int> current = mWorldLocation+mCharacterRelativePos;
    if(mUnmoveableTerrain.contains(getLandValue(next)))
    {
        if(mUnmoveableTerrain.contains(getLandValue(current))) //this is so we can escape if we end up in an unmoveable block
        {
            return true;
        }
        return false;
    }
    return true;
}

QString World::plantPicked()
{
    mPickedPlants.enqueue(mWorldLocation+mCharacterRelativePos);
    if(mPickedPlants.size()>1000)
        {
            mPickedPlants.dequeue();
        }
    return mCurrentLandNameIndex[getLandValue(mWorldLocation+mCharacterRelativePos)];
}

int World::getLandValue(std::complex<int> pos)
{
    return map.getValue(pos.real(),pos.imag())%mLandCount;
}

unsigned int World::complexHash(std::complex<int> input)
{

    std::hash<double> k{};
    std::complex<double> temp = std::pow(std::complex<double>(input.real(),input.imag()),2.3);

    return k(temp.real())+k(temp.imag())+k(std::abs(temp)) + k(std::arg(temp));
}

int World::getworldNum() const
{
    return mWorldNum;
}

std::complex<int> World::getCharecterLocation() const
{
    return mWorldLocation+mCharacterRelativePos;
}

bool World::plantAtSpot(std::complex<int> pos)
{
    unsigned int temp =complexHash(pos);

    if(0==temp%10)
    {
        if(!mPickedPlants.contains(pos))
        {
            return true;
        }
    }
    return false;
}

void World::setCharectorLocation(std::complex<int> pos)
{
    mWorldLocation=pos-mCharacterRelativePos;
}
