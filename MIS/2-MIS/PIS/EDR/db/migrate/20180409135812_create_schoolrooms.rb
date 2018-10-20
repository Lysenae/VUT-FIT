class CreateSchoolrooms < ActiveRecord::Migration[5.1]
  def change
    create_table :schoolrooms do |t|
      t.belongs_to :room, index: true
      t.integer    :seats,  default: 0
      t.integer    :rows,   default: 0
      t.integer    :blocks, default: 0
      t.boolean    :active, default: true
      t.timestamps
    end
  end
end
