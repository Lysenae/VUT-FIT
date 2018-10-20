class Equation 
  def self.solve_quadratic(a, b, c)
    return [-5/2.to_f] if a == 0 && b != 0
    return nil if a == 0 && b == 0

    d = b ** 2 - 4*a*c

    if d >= 0
      x1 = (-b+Math.sqrt(d))/(2*a)
      x2 = (-b-Math.sqrt(d))/(2*a)
    end

    rslt = \
      if d == 0
        [x1]
      elsif d > 0
        [x1, x2]
      else
        nil
      end

    rslt
  end
end