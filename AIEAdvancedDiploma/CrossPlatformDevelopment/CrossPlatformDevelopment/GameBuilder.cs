////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameBuilder.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatformDevelopment
{
    public class GameBuilder
    {
        /// <summary>
        /// Initialises the game objects
        /// </summary>
        public static void Build(SpriteBatch spriteBatch, ContentManager content, GameData data)
        {
            for(int i = 0; i < ID.MAX_SPRITES; ++i)
            {
                data.Sprites.Insert(i, new Sprite());
            }

            for (int i = 0; i < ID.MAX_TEXT; ++i)
            {
                data.Text.Insert(i, new Text());
            }

            data.Sprites[ID.MENU_BACKDROP].Load(spriteBatch, content, "menu");
            data.Sprites[ID.MENU_BACKDROP].SetSize(800, 600);

            data.Sprites[ID.GAME_BACKDROP].Load(spriteBatch, content, "game");
            data.Sprites[ID.GAME_BACKDROP].SetSize(800, 600);

            data.Sprites[ID.HIGH_SCORE_BACKDROP].Load(spriteBatch, content, "highscore");
            data.Sprites[ID.HIGH_SCORE_BACKDROP].SetSize(800, 600);

            data.Sprites[ID.PLAYER].Load(spriteBatch, content, "player");
            data.Sprites[ID.PLAYER].SetSize(100, 100);

            data.Sprites[ID.ENEMY].Load(spriteBatch, content, "enemy");
            data.Sprites[ID.ENEMY].SetSize(100, 100);

            data.Text[ID.SCORE].Load(spriteBatch, content, "Calibri_14");
            data.Text[ID.SCORE].SetText("HELLO!");
        }
    }
}
