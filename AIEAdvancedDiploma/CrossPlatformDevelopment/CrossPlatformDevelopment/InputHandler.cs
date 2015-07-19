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
    /// Handles any user input
    /// </summary>
    public class InputHandler
    {
        /// <summary>
        /// Information for the state of a key
        /// </summary>
        public class KeyState
        {
            public Action ActionCallback = null;
            public bool IsKeyDown = false;
        }

        Dictionary<Keys, KeyState> m_keyCallbacks;  ///< Map of key callbacks

        /// <summary>
        /// Constructor
        /// </summary>
        public InputHandler()
        {
            m_keyCallbacks = new Dictionary<Keys, KeyState>();
            InputPaused = false;
            IsMouseClicked = false;
            IsMouseDown = false;
        }

        /// <summary>
        /// Whether the mouse has been pressed and released
        /// </summary>
        public bool IsMouseClicked
        {
            get;
            set;
        }

        /// <summary>
        /// Whether the mouse is being pressed down
        /// </summary>
        public bool IsMouseDown
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to pause any user input
        /// </summary>
        public bool InputPaused 
        { 
            get; 
            set; 
        }

        /// <summary>
        /// Adds a new key callback
        /// </summary>
        public void AddCallback(Keys key, Action callback)
        {
            m_keyCallbacks[key] = new KeyState();
            m_keyCallbacks[key].ActionCallback = callback;
            m_keyCallbacks[key].IsKeyDown = false;
        }

        /// <summary>
        /// Updates user input
        /// </summary>
        public void Update()
        {
            bool isMouseDown = Mouse.GetState().LeftButton == ButtonState.Pressed;
            IsMouseClicked = IsMouseDown && !isMouseDown;
            IsMouseDown = isMouseDown;

            foreach (var pair in m_keyCallbacks)
            {
                bool isKeyDown = Keyboard.GetState().IsKeyDown(pair.Key);
                if (pair.Value.IsKeyDown && !isKeyDown)
                {
                    pair.Value.ActionCallback();
                }
                pair.Value.IsKeyDown = isKeyDown;
            }
        }
    }
}
