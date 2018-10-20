class Failure < ApplicationRecord
  belongs_to :device
  belongs_to :technician,   class_name: :Employee, foreign_key: :technician_id, optional: true
  belongs_to :announcer,    class_name: :Employee, foreign_key: :announcer_id
  validates  :announced_at, presence: true
  validates  :description,  presence: true

  def self.all_active
    self.where(active: true)
  end
end
