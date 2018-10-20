class SchoolroomDecorator < RoomBaseDecorator
  def get_seats
    seats ? seats : 0
  end

  def get_rows
    rows ? rows : 0
  end

  def get_blocks
    blocks ? blocks : 0
  end
end
