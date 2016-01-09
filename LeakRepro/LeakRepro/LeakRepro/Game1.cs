using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Info;

namespace LeakRepro
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            graphics.IsFullScreen = true;
            graphics.SupportedOrientations = DisplayOrientation.LandscapeRight;

            TargetElapsedTime = TimeSpan.FromTicks(333333);

            InactiveSleepTime = TimeSpan.FromSeconds(1);
        }

        protected override void Initialize()
        {
            TouchPanel.EnabledGestures = GestureType.Tap;

            base.Initialize();
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            while (TouchPanel.IsGestureAvailable)
            {
                var gs = TouchPanel.ReadGesture();
                if (gs.GestureType == GestureType.Tap)
                    LeakRepro();
            }
            
            base.Update(gameTime);
        }

        void LeakRepro()
        {
            for (int i = 0; i < 50; i++)
            {
                var rt = new RenderTarget2D(
                    GraphicsDevice,
                    800, 480,
                    false, SurfaceFormat.Color,
                    DepthFormat.None, 0,
                    RenderTargetUsage.PreserveContents);
                GraphicsDevice.SetRenderTarget(rt);
                GraphicsDevice.Clear(Color.Green);
                GraphicsDevice.SetRenderTarget(null);

                MemoryStream ms = new MemoryStream();
                rt.SaveAsPng(ms, 800, 480);
                ms.Close();
                rt.Dispose();
            }
            GC.Collect();
            List<string> buttons = new List<string>();
            buttons.Add("Close");
            var message = string.Format(
                "Total memory: {0}\n" +
                "Used memory: {1}\n",
                DeviceExtendedProperties.GetValue("DeviceTotalMemory").ToString(),
                DeviceExtendedProperties.GetValue("ApplicationCurrentMemoryUsage").ToString()
                );
            IAsyncResult ar = Guide.BeginShowMessageBox("Info",
                message,
                buttons, 0,
                MessageBoxIcon.None, null, null);
            Guide.EndShowMessageBox(ar);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            base.Draw(gameTime);
        }
    }
}
