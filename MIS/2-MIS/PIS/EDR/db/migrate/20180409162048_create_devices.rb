class CreateDevices < ActiveRecord::Migration[5.1]
  def change
    create_table :devices do |t|
      t.belongs_to :room,     index: true
      t.belongs_to :employee, index: true
      t.string     :brand, null: false
      t.string     :model, null: false
      t.decimal    :price, precision: 8, scale: 2
      t.text       :specification
      t.string     :status
      t.boolean		 :active, default: true
      t.timestamps
    end
  end
end
