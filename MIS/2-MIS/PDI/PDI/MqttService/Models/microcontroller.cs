namespace MqttService.Models
{
    public sealed class Microcontroller : Device
    {
        public double Temperature { get; set; }

        public Microcontroller(string id, bool powered = true, double temperature = 0.0)
        : base(id, powered)
        {
            this.Temperature = temperature;
        }
    }
}
