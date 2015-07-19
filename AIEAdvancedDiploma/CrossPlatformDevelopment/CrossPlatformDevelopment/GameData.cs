﻿////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameData.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;

namespace CrossPlatformDevelopment
{
    struct ID
    {
        /// <summary>
        /// Sprite IDs
        /// </summary>
        public const int MENU_BACKDROP = 0;
        public const int GAME_BACKDROP = 1;
        public const int HIGH_SCORE_BACKDROP = 2;
        public const int PLAYER = 3;
        public const int ENEMY = 4;
        public const int MAX_SPRITES = 5;

        /// <summary>
        /// Text IDs
        /// </summary>
        public const int SCORE = 0;
        public const int MAX_TEXT = 1;
    };

    /// <summary>
    /// Holds objects from the game
    /// </summary>
    public class GameData
    {
        public List<Sprite> Sprites = new List<Sprite>();  ///< Container of all sprites to draw
        public List<Text> Text = new List<Text>();         ///< Container of all text to draw
    }
}