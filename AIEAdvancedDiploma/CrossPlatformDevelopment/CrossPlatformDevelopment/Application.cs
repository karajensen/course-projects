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
        Fader m_stateFader;                 ///< Transitions the fade in/out of a new state
                                                   
        /// <summary>
        /// Constructor
        /// </summary>
        public Application()
        {
            m_data = new GameData();
            m_data.ChangeState = state => ChangeState(state);
            m_data.QuitGame = () => Exit();

            m_stateFader = new Fader();
            m_input = new InputHandler();
            m_game = new GamePlay(m_data);
            m_menu = new Menu(m_data);
            m_graphics = new GraphicsDeviceManager(this);

            m_stateFader.Speed = 2.0f;
            m_data.Width = Window.ClientBounds.Width;
            m_data.Height = Window.ClientBounds.Height;
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
            ChangeState(GameState.MENU);

            m_input.AddCallback(Keys.Escape, () => Exit());
            m_input.AddCallback(Keys.D, () => m_data.Text[ID.DIAGNOSTICS].ToggleVisible());
        }

        /// <summary>
        /// Changes to a new game state
        /// </summary>
        private void ChangeState(GameState state)
        {
            if(m_data.NextState == GameState.NONE)
            {
                m_data.NextState = state;
                m_stateFader.State = Fader.FadeState.FADE_OUT;
                m_input.InputPaused = true;
            }
        }

        /// <summary>
        /// Updates the state changing
        /// </summary>
        private void UpdateState(float deltatime)
        {
            if(m_stateFader.State != Fader.FadeState.NONE)
            {
                m_stateFader.Update(deltatime);

                float value = m_stateFader.Value;
                m_data.Sprites.ForEach(sprite => sprite.SetTransparency(value));
                m_data.Text.ForEach(text => text.SetTransparency(value));

                if(m_stateFader.State == Fader.FadeState.NONE && value == 0.0f)
                {
                    m_input.InputPaused = false;
                    m_stateFader.State = Fader.FadeState.FADE_IN;
                    m_data.CurrentState = m_data.NextState;
                    m_data.NextState = GameState.NONE;
                    m_data.Sprites.ForEach(sprite => sprite.SetVisible(false));

                    switch (m_data.CurrentState)
                    {
                        case GameState.MENU:
                            m_menu.Load();
                            break;
                        case GameState.GAME:
                            m_game.Load();
                            break;
                    }
                }
            }
        }

        /// <summary>
        /// Updates diagnostics
        /// </summary>
        private void UpdateDiagnostics(float deltatime)
        {
            if(m_data.Text[ID.DIAGNOSTICS].IsVisible())
            {
                int x = m_input.MousePosition.X;
                int y = m_input.MousePosition.Y;

                m_data.Text[ID.DIAGNOSTICS].SetText(
                    "Deltatime: " + deltatime.ToString() + "\n" +
                    "Mouse Pos: " + x.ToString() + ", " + y.ToString() + "\n" +
                    "Mouse Down: " + m_input.IsMouseDown.ToString() + "\n");
            }
        }

        /// <summary>
        /// Ticks the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values</param>
        protected override void Update(GameTime gameTime)
        {
            float deltatime = (float)gameTime.ElapsedGameTime.TotalSeconds;

            base.Update(gameTime);
            m_input.Update();
            UpdateState(deltatime);
            UpdateDiagnostics(deltatime);

            switch (m_data.CurrentState)
            {
            case GameState.MENU:
                m_menu.Update(deltatime, m_input);
                break;
            case GameState.GAME:
                m_game.Update(deltatime);
                break;
            }
        }

        /// <summary>
        /// Renders the game
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, Color.White, 0.0f, 0);
            m_spriteBatch.Begin(SpriteSortMode.FrontToBack, BlendState.NonPremultiplied);

            m_data.Sprites.ForEach(sprite => sprite.Render());
            m_data.Text.ForEach(text => text.Render());

            m_spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
