class CreateRooms < ActiveRecord::Migration[5.1]
  def change
    create_table :rooms do |t| 
      t.belongs_to :department, index: true
      t.string     :label, null: false
      t.string     :location
      t.decimal    :width,   precision: 3, scale: 2, default: 0.0
      t.decimal    :length,  precision: 3, scale: 2, default: 0.0
      t.decimal    :height,  precision: 3, scale: 2, default: 0.0
      t.boolean    :active, default: true
      t.timestamps
    end
  end
end
