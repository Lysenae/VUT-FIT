namespace MqttService.Models
{
    public abstract class Device
    {
        public string DeviceId { get; set; }
        public bool Powered { get; set; }

        public Device(string id, bool powered)
        {
            this.DeviceId = id;
            this.Powered  = powered;
        }
    }
}
