#include "Arrow.h"

namespace HuntTheWumpus
{
    Arrow::Arrow(const int arrowInstance, Context& providers)
        : Denizen(DenizenIdentifier{ .m_category = Category::Arrow, .m_instance = arrowInstance }
            , { .m_carryableByBats = false, .m_fatalToWumpus = true, .m_fatalToHunter = true, .m_isEdible = false, .m_reportMovement = false }
            , providers)
    {
    }
}
