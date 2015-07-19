////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Menu.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace CrossPlatformDevelopment
{
    /// <summary>
    /// Manages menu logic
    /// </summary>
    public class Menu
    {
        GameData m_data;  ///< Holds objects from the game

        /// <summary>
        /// Constructor
        /// </summary>
        public Menu(GameData data)
        {
            m_data = data;
        }

        /// <summary>
        /// Loads the menu
        /// </summary>
        public void Load()
        {
            m_data.Sprites[ID.MENU_BACKDROP].SetVisible(true);
        }

        /// <summary>
        /// Updates the menu
        /// </summary>
        public void Update()
        {
        }
    }
}
