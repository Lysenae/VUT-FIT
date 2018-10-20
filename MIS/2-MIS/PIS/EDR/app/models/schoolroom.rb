class Schoolroom < RoomBase
  def seats_per_row
    rows > 0 ? (seats / rows).to_i : nil
  end
end
