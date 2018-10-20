class CreateTransfers < ActiveRecord::Migration[5.1]
  def change
    create_table :transfers do |t|
      t.belongs_to :device,       index: true
      t.belongs_to :author,       index: true
      t.belongs_to :approved,     index: true
      t.belongs_to :from,         index: true
      t.belongs_to :to,           index: true
      t.string     :reason,       null: false
      t.datetime   :requested_at, null: false
      t.datetime   :done_at
      t.boolean    :active, default: true
      t.timestamps
    end
  end
end
