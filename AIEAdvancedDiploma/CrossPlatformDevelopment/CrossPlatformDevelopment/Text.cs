////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Text.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CrossPlatformDevelopment
{
    /// <summary>
    /// Manages renderable text
    /// </summary>
    public class Text
    {
        SpriteBatch m_renderer;                          ///< Allows rendering of sprites
        String m_text = "";                              ///< Text to render
        SpriteFont m_font;                               ///< The font style to use
        Vector2 m_position = new Vector2();              ///< The position of the text
        SpriteEffects m_flipState = SpriteEffects.None;  ///< Whether flipped or normal
        Color m_colour = Color.White;                    ///< The colour of the text
        float m_depth = 0.0f;                            ///< The depth of the text from 0-1

        /// <summary>
        /// Constructor
        /// </summary>
        public Text()
        {
        }

        /// <summary>
        /// Loads the text
        /// </summary>
        public void Load(SpriteBatch renderer, ContentManager content, String name)
        {
            m_renderer = renderer;
            m_font = content.Load<SpriteFont>(name);
        }

        /// <summary>
        /// Sets the position of the text
        /// </summary>
        public void SetPosition(int x, int y)
        {
            m_position = new Vector2(x, y);
        }

        /// <summary>
        /// Sets the colour of the text
        /// </summary>
        public void SetColour(Color colour)
        {
            m_colour = colour;
        }

        /// <summary>
        /// Sets what the text should render
        /// </summary>
        public void SetText(String text)
        {
            m_text = text;
        }

        /// <summary>
        /// Sets the depth of the sprite
        /// </summary>
        public void SetDepth(float depth)
        {
            m_depth = depth;
        }

        /// <summary>
        /// Renders the text
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public void Render(GameTime gameTime)
        {
            m_renderer.DrawString(m_font, m_text, m_position, m_colour, 
                0.0f, Vector2.Zero, 3.0f, m_flipState, m_depth);
        }
    }
}
