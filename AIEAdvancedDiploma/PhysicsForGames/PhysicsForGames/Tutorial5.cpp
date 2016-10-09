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

    const float size = 20.0f;
    const glm::vec2 position(m_data->Size().x * 0.5f, m_data->Size().y - 100.0f);
    for (int i = 0; i < 5; ++i)
    {
        std::unique_ptr<SquareBody> square(new SquareBody(
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0, 0), 5.0f,
            glm::vec2(size, size),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

        switch (i)
        {
        case 0:
            square->SetPosition(position);
            break;
        case 1:
            square->SetPosition(position.x - size, position.y - size);
            break;
        case 2:
            square->SetPosition(position.x - size, position.y + size);
            break;
        case 3:
            square->SetPosition(position.x + size, position.y - size);
            break;
        case 4:
            square->SetPosition(position.x + size, position.y + size);
            break;
        }

        square->SetRotation(DegToRad(float(rand() % 361)));
        m_data->Scene().AddActor(std::move(square));
    }
}