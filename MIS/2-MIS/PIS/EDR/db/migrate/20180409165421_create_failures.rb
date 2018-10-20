class CreateFailures < ActiveRecord::Migration[5.1]
  def change
    create_table :failures do |t|
      t.belongs_to :device,       index: true
      t.belongs_to :technician,   index: true
      t.belongs_to :announcer,    index: true
      t.text       :description,  null: false
      t.datetime   :announced_at, null: false
      t.boolean    :solved,       default: false
      t.text       :report
			t.boolean    :active, default: true
      t.timestamps
    end
  end
end
