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
        /// <summary>
        /// The current state of the game
        /// </summary>
        enum GameState
        {
            MENU,
            GAME,
            HIGH_SCORE,
            GAME_OVER
        };

        GameState m_gameState = GameState.MENU;   ///< The current state of the game
        GraphicsDeviceManager m_graphics;         ///< Manages graphics for the game
        SpriteBatch m_spriteBatch;                ///< Allows rendering of 2D objects

        Sprite m_sampleSprite;
        Text m_sampleText;

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

            m_sampleText = new Text();
            m_sampleText.Load(m_spriteBatch, Content, "Calibri_14");
            m_sampleText.SetText("Hello!");
            m_sampleText.SetColour(Color.Red);
            m_sampleText.SetDepth(0.0f);

            m_sampleSprite = new Sprite();
            m_sampleSprite.Load(m_spriteBatch, Content, "player");
            m_sampleSprite.SetSize(100, 100);
            m_sampleSprite.SetDepth(1.0f);
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
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, Color.Black, 0.0f, 0);
            m_spriteBatch.Begin();

            m_sampleText.Render(gameTime);
            m_sampleSprite.Render(gameTime);

            m_spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
