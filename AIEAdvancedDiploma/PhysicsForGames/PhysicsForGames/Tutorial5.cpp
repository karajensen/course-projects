////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tutorial5.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TutorialCreator.h"
#include "TutorialData.h"
#include "TutorialTweaker.h"
#include "PhysicsScene.h"
#include "SquareBody.h"
#include "PoolTable.h"
#include "Utilities.h"

void TutorialCreator::CreateTutorial5()
{
    PoolTable::Create(*m_data, true, true);
}