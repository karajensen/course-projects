////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionSolver.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using Microsoft.Xna.Framework;

namespace CrossPlatformDevelopment
{
    class Collision
    {
        /// <summary>
        /// Solves a collision between a point and rectangle
        /// </summary>
        public static bool HasCollision(Rectangle rect, int x, int y)
        {
            return x >= rect.Left && x <= rect.Right && y >= rect.Top && y <= rect.Bottom;
        }
    }
}
