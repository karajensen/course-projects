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
            data.Sprites[ID.MENU_BACKDROP].SetDepth(0.0f);

            data.Sprites[ID.MENU_PLAY].Load(spriteBatch, content, "menu_play");
            data.Sprites[ID.MENU_PLAY].SetSize(400, 100);
            data.Sprites[ID.MENU_PLAY].SetCenter(data.Width / 2, 150);
            data.Sprites[ID.MENU_PLAY].SetDepth(1.0f);

            data.Sprites[ID.MENU_SCORE].Load(spriteBatch, content, "menu_score");
            data.Sprites[ID.MENU_SCORE].SetSize(500, 100);
            data.Sprites[ID.MENU_SCORE].SetCenter(data.Width / 2, 300);
            data.Sprites[ID.MENU_SCORE].SetDepth(1.0f);

            data.Sprites[ID.MENU_QUIT].Load(spriteBatch, content, "menu_quit");
            data.Sprites[ID.MENU_QUIT].SetSize(200, 100);
            data.Sprites[ID.MENU_QUIT].SetCenter(data.Width / 2, 450);
            data.Sprites[ID.MENU_QUIT].SetDepth(1.0f);

            data.Sprites[ID.GAME_BACKDROP].Load(spriteBatch, content, "game");
            data.Sprites[ID.GAME_BACKDROP].SetSize(800, 600);
            data.Sprites[ID.GAME_BACKDROP].SetDepth(0.0f);

            data.Sprites[ID.HIGH_SCORE_BACKDROP].Load(spriteBatch, content, "highscore");
            data.Sprites[ID.HIGH_SCORE_BACKDROP].SetSize(800, 600);
            data.Sprites[ID.HIGH_SCORE_BACKDROP].SetDepth(0.0f);

            data.Sprites[ID.PLAYER].Load(spriteBatch, content, "player");
            data.Sprites[ID.PLAYER].SetSize(100, 100);
            data.Sprites[ID.PLAYER].SetDepth(0.5f);

            data.Sprites[ID.ENEMY].Load(spriteBatch, content, "enemy");
            data.Sprites[ID.ENEMY].SetSize(100, 100);
            data.Sprites[ID.ENEMY].SetDepth(0.4f);

            data.Text[ID.DIAGNOSTICS].Load(spriteBatch, content, "Calibri_14");
            data.Text[ID.DIAGNOSTICS].SetDepth(1.0f);
            data.Text[ID.DIAGNOSTICS].SetColour(Color.Black);
        }
    }
}
