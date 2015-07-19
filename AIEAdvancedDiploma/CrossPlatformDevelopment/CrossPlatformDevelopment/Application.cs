////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Application.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

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

        GameState m_gameState = GameState.MENU;  ///< The current state of the game
        GameData m_data = new GameData();        ///< Holds objects from the game
        GraphicsDeviceManager m_graphics;        ///< Manages graphics for the game
        SpriteBatch m_spriteBatch;               ///< Allows rendering of 2D objects
        Menu m_menu;                             ///< Manages menu logic
        GamePlay m_game;                         ///< Manages game logic

        /// <summary>
        /// Constructor
        /// </summary>
        public Application()
        {
            m_game = new GamePlay(m_data);
            m_menu = new Menu(m_data);
            m_graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        /// <summary>
        /// Initialistaion once on application startup
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
                m_data.Sprites.Insert(i, new Sprite());
            }

            for (int i = 0; i < ID.MAX_TEXT; ++i)
            {
                m_data.Text.Insert(i, new Text());
            }

            m_data.Sprites[ID.MENU_BACKDROP].Load(m_spriteBatch, Content, "menu");
            m_data.Sprites[ID.MENU_BACKDROP].SetSize(800, 600);

            m_data.Sprites[ID.GAME_BACKDROP].Load(m_spriteBatch, Content, "game");
            m_data.Sprites[ID.GAME_BACKDROP].SetSize(800, 600);

            m_data.Sprites[ID.HIGH_SCORE_BACKDROP].Load(m_spriteBatch, Content, "highscore");
            m_data.Sprites[ID.HIGH_SCORE_BACKDROP].SetSize(800, 600);

            m_data.Sprites[ID.PLAYER].Load(m_spriteBatch, Content, "player");
            m_data.Sprites[ID.PLAYER].SetSize(100, 100);

            m_data.Sprites[ID.ENEMY].Load(m_spriteBatch, Content, "enemy");
            m_data.Sprites[ID.ENEMY].SetSize(100, 100);

            m_data.Text[ID.SCORE].Load(m_spriteBatch, Content, "Calibri_14");
            m_data.Text[ID.SCORE].SetText("HELLO!");

            ChangeState(GameState.GAME);
        }

        /// <summary>
        /// Changes to a new game state
        /// </summary>
        private void ChangeState(GameState state)
        {
            m_gameState = state;
            m_data.Sprites.ForEach(sprite => sprite.SetVisible(false));

            switch (m_gameState)
            {
            case GameState.MENU:
                m_menu.Load();
                break;
            case GameState.GAME:
                m_game.Load(Window);
                break;
            }
        }

        /// <summary>
        /// Ticks the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values</param>
        protected override void Update(GameTime gameTime)
        {
            base.Update(gameTime);

            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || 
                Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                Exit();
            }

            TouchCollection touches = TouchPanel.GetState();
            foreach (TouchLocation touch in touches)
            {
                if (touch.State == TouchLocationState.Pressed)
                {
                    ChangeState(GameState.MENU);
                }
            }

            switch(m_gameState)
            {
            case GameState.MENU:
                m_menu.Update();
                break;
            case GameState.GAME:
                m_game.Update();
                break;
            }
        }

        /// <summary>
        /// Renders the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, Color.Black, 0.0f, 0);
            m_spriteBatch.Begin();

            m_data.Sprites.ForEach(sprite => sprite.Render(gameTime));
            m_data.Text.ForEach(text => text.Render(gameTime));

            m_spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
