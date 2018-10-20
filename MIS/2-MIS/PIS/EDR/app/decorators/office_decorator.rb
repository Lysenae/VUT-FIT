class OfficeDecorator < RoomBaseDecorator
  def el_sockets
    electrical_sockets ? electrical_sockets : 0
  end

  def eth_sockets
    ethernet_sockets ? ethernet_sockets : 0
  end
end
