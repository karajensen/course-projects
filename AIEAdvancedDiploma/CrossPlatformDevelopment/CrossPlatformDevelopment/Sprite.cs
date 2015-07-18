////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sprite.cs
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
    /// Manages a renderable texture
    /// </summary>
    public class Sprite
    {
        SpriteBatch m_renderer;                          ///< Allows rendering of sprites
        Texture2D m_texture;                             ///< Texture to render
        SpriteEffects m_flipState = SpriteEffects.None;  ///< Whether flipped or normal
        Rectangle m_rectangle = new Rectangle();         ///< The dimension of the sprite   
        float m_depth = 0.0f;                            ///< The depth of the sprite from 0-1

        /// <summary>
        /// Constructor
        /// </summary>
        public Sprite()
        {
        }

        /// <summary>
        /// Loads the sprite
        /// </summary>
        public void Load(SpriteBatch renderer, ContentManager content, String name)
        {
            m_renderer = renderer;
            m_texture = content.Load<Texture2D>(name);
        }

        /// <summary>
        /// Sets the size of the sprite
        /// </summary>
        public void SetSize(int width, int height)
        {
            m_rectangle.Width = width;
            m_rectangle.Height = height;
        }

        /// <summary>
        /// Sets the position of the sprite
        /// </summary>
        public void SetPosition(int x, int y)
        {
            m_rectangle.X = x;
            m_rectangle.Y = y;
        }

        /// <summary>
        /// Sets the depth of the sprite
        /// </summary>
        public void SetDepth(float depth)
        {
            m_depth = depth;
        }

        /// <summary>
        /// Renders the sprite
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public void Render(GameTime gameTime)
        {
            m_renderer.Draw(m_texture, m_rectangle, null, 
                Color.White, 0.0f, Vector2.Zero, m_flipState, m_depth);
        }
    }
}
