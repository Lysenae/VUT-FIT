class Room < ApplicationRecord
  belongs_to :department
  has_many   :devices
  validates  :label, presence: true
  validates  :label, uniqueness: true

  def self.all_active
    self.where(active: true)
  end
end
