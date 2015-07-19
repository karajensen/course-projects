////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Text.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatformDevelopment
{
    /// <summary>
    /// Manages renderable text
    /// </summary>
    public class Text
    {
        SpriteBatch m_renderer;                                   ///< Allows rendering of sprites
        SpriteFont m_font;                                        ///< The font style to use
        List<Instance> m_instances = new List<Instance>();        ///< Instances of the text

        /// <summary>
        /// Instance of the text
        /// </summary>
        class Instance
        {
            public String Text = "";                              ///< Text to render
            public Vector2 Position = new Vector2();              ///< The position of the text
            public SpriteEffects FlipState = SpriteEffects.None;  ///< Whether flipped or normal
            public Color Colour = new Color(Color.White, 1.0f);   ///< The colour of the text
            public float Depth = 0.0f;                            ///< The depth of the text from 0-1
            public bool Visible = true;                           ///< Whether to render the text
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Text()
        {
        }

        /// <summary>
        /// Loads the text and automatically creates a single instance
        /// </summary>
        public void Load(SpriteBatch renderer, ContentManager content, String name)
        {
            m_renderer = renderer;
            m_font = content.Load<SpriteFont>(name);
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
        /// Sets the position of the text
        /// </summary>
        public void SetPosition(int x, int y, int ID = 0)
        {
            m_instances[ID].Position = new Vector2(x, y);
        }

        /// <summary>
        /// Sets the colour of the text
        /// </summary>
        public void SetColour(Color colour, int ID = 0)
        {
            m_instances[ID].Colour.R = colour.R;
            m_instances[ID].Colour.G = colour.G;
            m_instances[ID].Colour.B = colour.B;
        }

        /// <summary>
        /// Sets what the text should render
        /// </summary>
        public void SetText(String text, int ID = 0)
        {
            m_instances[ID].Text = text;
        }

        /// <summary>
        /// Sets the depth of the sprite
        /// </summary>
        public void SetDepth(float depth, int ID = 0)
        {
            m_instances[ID].Depth = depth;
        }

        /// <summary>
        /// Sets whether the text is visible
        /// </summary>
        public void SetVisible(bool visible, int ID = 0)
        {
            m_instances[ID].Visible = visible;
        }

        /// <summary>
        /// Sets the transparency of the text
        /// </summary>
        public void SetTransparency(float value, int ID = 0)
        {
            m_instances[ID].Colour.A = (byte)Math.Max(Math.Min(255, (int)value * 255), 0);
        }

        /// <summary>
        /// Renders the text
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        public void Render(GameTime gameTime)
        {
            m_instances.ForEach(instance =>
            {
                if (instance.Visible)
                {
                    m_renderer.DrawString(m_font, instance.Text, instance.Position, instance.Colour,
                        0.0f, Vector2.Zero, 1.0f, instance.FlipState, instance.Depth);
                }
            });
        }
    }
}
