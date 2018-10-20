class TransferDecorator < ApplicationDecorator
  delegate_all

  def finished_str
    finished? ? 'Yes' : 'No'
  end

  def request_date
    tm = requested_at ? Time.parse(requested_at.to_s) : Time.now
    tm.strftime('%Y-%m-%dT%H:%M')
  end

  def done_date
    done_at ? Time.parse(done_at.to_s).strftime('%Y-%m-%dT%H:%M') : nil
  end
end
