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
        GameData m_data;                    ///< Holds objects from the game
        InputHandler m_input;               ///< Handles user input
        GraphicsDeviceManager m_graphics;   ///< Manages graphics for the game
        SpriteBatch m_spriteBatch;          ///< Allows rendering of 2D objects
        Menu m_menu;                        ///< Manages menu logic
        GamePlay m_game;                    ///< Manages game logic
                                                   
        /// <summary>
        /// Constructor
        /// </summary>
        public Application()
        {
            m_data = new GameData();
            m_data.ChangeState = state => ChangeState(state);

            m_input = new InputHandler();
            m_game = new GamePlay(m_data);
            m_menu = new Menu(m_data);
            m_graphics = new GraphicsDeviceManager(this);

            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        /// <summary>
        /// Load game graphics/content
        /// </summary>
        protected override void LoadContent()
        {
            m_spriteBatch = new SpriteBatch(GraphicsDevice);
            GameBuilder.Build(m_spriteBatch, Content, m_data);
            ChangeState(GameState.GAME);

            m_input.AddCallback(Keys.Escape, () => Exit());
        }

        /// <summary>
        /// Changes to a new game state
        /// </summary>
        private void ChangeState(GameState state)
        {
            m_data.CurrentState = state;
            m_data.Sprites.ForEach(sprite => sprite.SetVisible(false));

            switch (m_data.CurrentState)
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
            m_input.Update();

            switch (m_data.CurrentState)
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
