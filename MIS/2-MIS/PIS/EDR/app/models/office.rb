class Office < RoomBase
  before_destroy :update_employees

  def update_employees
    Employee.where(office_id: id).each do |e|
      e.update_attribute(:office_id, nil)
    end
  end

  def room_label
    room.label if room
  end
end
