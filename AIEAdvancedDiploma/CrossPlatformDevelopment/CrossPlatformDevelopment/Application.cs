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

        GameState m_gameState = GameState.MENU;       ///< The current state of the game
        GraphicsDeviceManager m_graphics;             ///< Manages graphics for the game
        SpriteBatch m_spriteBatch;                    ///< Allows rendering of 2D objects

        List<Sprite> m_sprites = new List<Sprite>();  ///< Container of all sprites to draw
        List<Text> m_text = new List<Text>();         ///< Container of all text to draw

        /// <summary>
        /// Constructor
        /// </summary>
        public Application() 
            : base()
        {
            m_graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
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

            for(int i = 0; i < ID.MAX_SPRITES; ++i)
            {
                m_sprites.Insert(i, new Sprite());
            }

            for (int i = 0; i < ID.MAX_TEXT; ++i)
            {
                m_text.Insert(i, new Text());
            }

            m_sprites[ID.MENU_BACKDROP].Load(m_spriteBatch, Content, "menu");
            m_sprites[ID.MENU_BACKDROP].SetSize(800, 600);

            m_sprites[ID.GAME_BACKDROP].Load(m_spriteBatch, Content, "game");
            m_sprites[ID.GAME_BACKDROP].SetSize(800, 600);

            m_sprites[ID.HIGH_SCORE_BACKDROP].Load(m_spriteBatch, Content, "highscore");
            m_sprites[ID.HIGH_SCORE_BACKDROP].SetSize(800, 600);
            
            m_sprites[ID.PLAYER].Load(m_spriteBatch, Content, "player");
            m_sprites[ID.PLAYER].SetSize(100, 100);

            m_sprites[ID.ENEMY].Load(m_spriteBatch, Content, "enemy");
            m_sprites[ID.ENEMY].SetSize(100, 100);

            m_text[ID.SCORE].Load(m_spriteBatch, Content, "Calibri_14");
            m_text[ID.SCORE].SetText("HELLO!");

            ChangeState(GameState.GAME);
        }

        /// <summary>
        /// Resets to a new game
        /// </summary>
        private void ResetGame()
        {
            m_sprites[ID.PLAYER].SetCenter(Window.ClientBounds.Width / 2, Window.ClientBounds.Height / 2);
        }

        /// <summary>
        /// Changes to a new game state
        /// </summary>
        private void ChangeState(GameState state)
        {
            m_gameState = state;

            m_sprites.ForEach(sprite => sprite.SetVisible(false));

            if(m_gameState == GameState.MENU)
            {
                m_sprites[ID.MENU_BACKDROP].SetVisible(true);
            }
            else if(m_gameState == GameState.HIGH_SCORE)
            {
                m_sprites[ID.HIGH_SCORE_BACKDROP].SetVisible(true);
            }
            else if (m_gameState == GameState.GAME_OVER)
            {
            }
            else /*GameState.GAME*/
            {
                m_sprites[ID.GAME_BACKDROP].SetVisible(true);
                m_sprites[ID.PLAYER].SetVisible(true);
                m_sprites[ID.ENEMY].SetVisible(true);
                ResetGame();
            }
        }

        /// <summary>
        /// Updates the menu
        /// </summary>
        private void UpdateMenu()
        {
        }

        /// <summary>
        /// Updates the game
        /// </summary>
        private void UpdateGame()
        {
        }

        /// <summary>
        /// Updates the high score
        /// </summary>
        private void UpdateHighScore()
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

            switch(m_gameState)
            {
            case GameState.MENU:
                UpdateMenu();
                break;
            case GameState.GAME:
                UpdateGame();
                break;
            case GameState.HIGH_SCORE:
                UpdateHighScore();
                break;
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

            m_sprites.ForEach(sprite => sprite.Render(gameTime));
            m_text.ForEach(text => text.Render(gameTime));

            m_spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
