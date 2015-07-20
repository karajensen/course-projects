////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GamePlay.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace CrossPlatformDevelopment
{
    /// <summary>
    /// Manages game logic
    /// </summary>
    public class GamePlay
    {
        GameData m_data;  ///< Holds objects from the game

        /// <summary>
        /// Constructor
        /// </summary>
        public GamePlay(GameData data)
        {
            m_data = data;
        }

        /// <summary>
        /// Loads the game
        /// </summary>
        public void Load()
        {
            m_data.Sprites[ID.GAME_BACKDROP].SetVisible(true);
            m_data.Sprites[ID.PLAYER].SetVisible(true);
            m_data.Sprites[ID.ENEMY].SetVisible(true);

            m_data.Sprites[ID.PLAYER].SetCenter(
                m_data.Width / 2, m_data.Height / 2);            
        }

        /// <summary>
        /// Updates the game
        /// </summary>
        public void Update(float deltatime)
        {
        }
    }
}
