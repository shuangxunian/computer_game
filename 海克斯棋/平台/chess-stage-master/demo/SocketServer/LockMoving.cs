using System;

namespace SocketServer
{
    internal class LockMoving
    {
        private Boolean lockV;
        public LockMoving(Boolean b)
        {
            lockV = b;
        }

        public bool LockV { get => lockV; set => lockV = value; }
    }
}