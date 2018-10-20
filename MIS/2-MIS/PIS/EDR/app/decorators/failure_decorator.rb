class FailureDecorator < Draper::Decorator
  delegate_all

  def solved_str
    solved ? 'Yes' : 'No'
  end

  def device_name
    device ? "#{device.brand} #{device.model}" : nil
  end

  def announcer_name
    person_name(announcer)
  end

  def technician_name
    person_name(technician)
  end

  def announce_date
    tm = announced_at ? Time.parse(announced_at.to_s) : Time.now
    tm.strftime('%Y-%m-%dT%H:%M')
  end

  private
    def person_name(person)
      person ? "#{person.name} #{person.surname}" : nil
    end
end
