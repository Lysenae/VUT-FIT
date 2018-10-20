namespace MqttService.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class new_columns : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.PowerStripEntities", "EnergyConsumption", c => c.Double(nullable: false));
            AddColumn("dbo.PowerStripEntities", "Date", c => c.DateTime(nullable: false));
        }
        
        public override void Down()
        {
            DropColumn("dbo.PowerStripEntities", "Date");
            DropColumn("dbo.PowerStripEntities", "EnergyConsumption");
        }
    }
}
