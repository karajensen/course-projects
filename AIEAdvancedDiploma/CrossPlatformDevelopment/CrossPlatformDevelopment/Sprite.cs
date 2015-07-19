////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sprite.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatformDevelopment
{
    /// <summary>
    /// Manages a renderable texture
    /// </summary>
    public class Sprite
    {
        SpriteBatch m_renderer;                                   ///< Allows rendering of sprites
        Texture2D m_texture;                                      ///< Texture to render
        List<Instance> m_instances = new List<Instance>();        ///< Instances of the sprite

        /// <summary>
        /// Instance of the sprite
        /// </summary>
        class Instance
        {
            public SpriteEffects FlipState = SpriteEffects.None;  ///< Whether flipped or normal
            public Rectangle Rectangle = new Rectangle();         ///< The dimension of the sprite   
            public float Depth = 0.0f;                            ///< The depth of the sprite from 0-1
            public bool Visible = true;                           ///< Whether to render the sprite
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Sprite()
        {
        }

        /// <summary>
        /// Loads the sprite and automatically creates an instance
        /// </summary>
        public void Load(SpriteBatch renderer, ContentManager content, String name)
        {
            m_renderer = renderer;
            m_texture = content.Load<Texture2D>(name);
            AddInstances(1);
        }

        /// <summary>
        /// Creates new instances
        /// </summary>
        public void AddInstances(int amount)
        {
            for (int i = 0; i < amount; ++i)
            {
                m_instances.Add(new Instance());
            }
        }

        /// <summary>
        /// Sets the size of the sprite
        /// </summary>
        public void SetSize(int width, int height, int ID = 0)
        {
            m_instances[ID].Rectangle.Width = width;
            m_instances[ID].Rectangle.Height = height;
        }

        /// <summary>
        /// Sets the position of the sprite from the top left corner
        /// </summary>
        public void SetPosition(int x, int y, int ID = 0)
        {
            m_instances[ID].Rectangle.X = x;
            m_instances[ID].Rectangle.Y = y;
        }

        /// <summary>
        /// Sets the position of the sprite from its center
        /// </summary>
        public void SetCenter(int x, int y, int ID = 0)
        {
            m_instances[ID].Rectangle.X = x - m_instances[ID].Rectangle.Width / 2;
            m_instances[ID].Rectangle.Y = y - m_instances[ID].Rectangle.Height;
        }

        /// <summary>
        /// Sets the depth of the sprite
        /// </summary>
        public void SetDepth(float depth, int ID = 0)
        {
            m_instances[ID].Depth = depth;
        }

        /// <summary>
        /// Sets whether the sprite is visible
        /// </summary>
        public void SetVisible(bool visible, int ID = 0)
        {
            m_instances[ID].Visible = visible;
        }

        /// <summary>
        /// Renders the sprite
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public void Render(GameTime gameTime)
        {
            m_instances.ForEach(instance =>
            {
                if (instance.Visible)
                {
                    m_renderer.Draw(m_texture, instance.Rectangle, null,
                        Color.White, 0.0f, Vector2.Zero, instance.FlipState, instance.Depth);
                }
            });
        }
    }
}
