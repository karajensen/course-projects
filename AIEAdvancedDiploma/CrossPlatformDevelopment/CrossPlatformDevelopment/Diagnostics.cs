////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Diagnostics.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatformDevelopment
{
    class Diagnostics
    {
        Text m_textSprite;                ///< Sprite rendering the text
        static string sm_text;            ///< Text to render
        static bool sm_visible = false;   ///< Whether the diagnostics are visible

        /// <summary>
        /// Loads the diagnostics
        /// </summary>
        public void Load(SpriteBatch renderer, ContentManager content)
        {
            m_textSprite = new Text();
            m_textSprite.Load(renderer, content, "Calibri_14");
            m_textSprite.SetDepth(1.0f);
            m_textSprite.SetColour(Color.Black);
        }

        /// <summary>
        /// Toggles whether the diagnostics are visible
        /// </summary>
        public void Toggle()
        {
            sm_visible = !sm_visible;
        }

        /// <summary>
        /// Returns whether the diagnostics are visible
        /// </summary>
        public bool IsVisible()
        {
            return sm_visible;
        }

        /// <summary>
        /// Resets the diagnostic text
        /// </summary>
        public void Clear()
        {
            sm_text = "";
        }
        
        /// <summary>
        /// Adds a new line to the text diagnostics
        /// </summary>
        public static void AddText(string title, string text)
        {
            sm_text += " " + title + ": " + text + "\n";
        }

        /// <summary>
        /// Renders the diagnostics
        /// </summary>
        public void Render()
        {
            if (sm_visible)
            {
                m_textSprite.SetText(sm_text);
                m_textSprite.Render();
            }
        }
    }
}
