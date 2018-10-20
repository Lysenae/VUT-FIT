class RoomBase < ApplicationRecord
  self.abstract_class = true
  belongs_to :room

  def departments
    Department.all_active
  end

  def self.all_active
    self.all.select { |x| x.room.active }
  end
end
