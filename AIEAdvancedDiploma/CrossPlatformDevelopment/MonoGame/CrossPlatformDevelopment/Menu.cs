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
            m_data.Sprites[ID.MENU_PLAY].SetVisible(true);
            m_data.Sprites[ID.MENU_SCORE].SetVisible(true);
            m_data.Sprites[ID.MENU_QUIT].SetVisible(true);
        }

        /// <summary>
        /// Updates the menu
        /// </summary>
        public void Update(float deltatime, InputHandler input)
        {
            if(input.IsMouseClicked)
            {
                int x = input.MousePosition.X;
                int y = input.MousePosition.Y;

                if(Collision.HasCollision(m_data.Sprites[ID.MENU_PLAY].GetRectangle(), x, y))
                {
                    m_data.ChangeState(GameState.GAME);
                }
                else if(Collision.HasCollision(m_data.Sprites[ID.MENU_SCORE].GetRectangle(), x, y))
                {
                    m_data.ChangeState(GameState.HIGH_SCORE);
                }
                else if(Collision.HasCollision(m_data.Sprites[ID.MENU_SCORE].GetRectangle(), x, y))
                {
                    m_data.QuitGame();
                }
            }
        }
    }
}
