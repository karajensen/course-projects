////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Fader.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using Microsoft.Xna.Framework;

namespace CrossPlatformDevelopment
{
    class Fader
    {
        /// <summary>
        /// States for fading
        /// </summary>
        public enum FadeState
        {
            FADE_IN,
            FADE_OUT,
            NONE
        };

        /// <summary>
        /// Constructor
        /// </summary>
        public Fader()
        {
            State = FadeState.NONE;
            Value = 0.0f;
            Speed = 1.0f;
        }

        /// <summary>
        /// The current state of the fade
        /// </summary>
        public FadeState State
        {
            get;
            set;
        }

        /// <summary>
        /// The value of the fade
        /// </summary>
        public float Value
        {
            get;
            private set;
        }

        /// <summary>
        /// The speed of the fade
        /// </summary>
        public float Speed
        {
            get;
            set;
        }

        /// <summary>
        /// Updates the fade state
        /// </summary>
        public void Update(float deltatime)
        {
            if (State == FadeState.FADE_IN)
            {
                Value += deltatime * Speed;
                Value = Math.Min(1.0f, Math.Max(0.0f, Value));

                if(Value == 1.0f)
                {
                    State = FadeState.NONE;
                }
            }
            else if (State == FadeState.FADE_OUT)
            {
                Value -= deltatime * Speed;
                Value = Math.Min(1.0f, Math.Max(0.0f, Value));

                if (Value == 0.0f)
                {
                    State = FadeState.NONE;
                }
            }
        }
    };
}
