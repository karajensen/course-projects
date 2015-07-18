////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Application.cs
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
    /// The main application instance
    /// </summary>
    public class Application : Game
    {
        GraphicsDeviceManager m_graphics;
        SpriteBatch m_spriteBatch;

        Texture2D m_sampleTexture;
        SpriteFont m_calibri_14;
        SpriteFont m_calibri_18;
        SpriteFont m_calibri_22;

        /// <summary>
        /// Constructor
        /// </summary>
        public Application() 
            : base()
        {
            m_graphics = new GraphicsDeviceManager(this);
            m_graphics.PreferredBackBufferWidth = Shared.WINDOW_WIDTH;
            m_graphics.PreferredBackBufferHeight = Shared.WINDOW_HEIGHT;

            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Game logic initialistaion
        /// </summary>
        protected override void Initialize()
        {
            base.Initialize();
        }

        /// <summary>
        /// Load game graphics/content
        /// </summary>
        protected override void LoadContent()
        {
            m_spriteBatch = new SpriteBatch(GraphicsDevice);

            m_calibri_14 = Content.Load<SpriteFont>("Calibri_14");
            m_calibri_18 = Content.Load<SpriteFont>("Calibri_18");
            m_calibri_22 = Content.Load<SpriteFont>("Calibri_22");

            m_sampleTexture = Content.Load<Texture2D>("texture");
        }

        /// <summary>
        /// Unload game graphics/content
        /// </summary>
        protected override void UnloadContent()
        {
        }

        /// <summary>
        /// Ticks the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || 
                Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                Exit();
            }

            base.Update(gameTime);
        }

        /// <summary>
        /// Renders the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            m_spriteBatch.Begin();

            m_spriteBatch.Draw(m_sampleTexture,
                new Rectangle(0, 0, 200, 200),
                null, Color.White, 0.0f, Vector2.Zero,
                SpriteEffects.FlipHorizontally, 0);

            float yPos = 10;
            float xPos = 10;
            m_spriteBatch.DrawString(m_calibri_14, "Size 14: Calibri", new Vector2(xPos, yPos), Color.White);
            yPos += m_calibri_14.LineSpacing;
            m_spriteBatch.DrawString(m_calibri_18, "Size 18: Calibri", new Vector2(xPos, yPos), Color.White);
            yPos += m_calibri_18.LineSpacing;
            m_spriteBatch.DrawString(m_calibri_22, "Size 22: Calibri", new Vector2(xPos, yPos), Color.White);

            m_spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
