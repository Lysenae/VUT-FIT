class FibonacciSequenceSlow
  # konstruktor, hodnoty prvnich dvou clenu lze nastavit (implicitne 0, 1)
  def initialize(f0 = 0, f1 = 1)
    @init = [f0, f1]
    @current = nil
    reset
  end
  
  # nastavi radu do vychoziho stavu 
  def reset
    @idx = -1
    return true
  end
  
  # vrati dalsi fibonacciho cislo
  def next
    @idx += 1
    current
  end
  
  # aktualni fibonacciho cislo
  def current
    return nil if @idx < 0
    fib(@idx)
  end
  
  # aktualni index (vraci nil, pokud se jeste nezacalo generovat)
  def current_idx
    return @idx >= 0 ? @idx : nil
  end
  
  # vrati fibonacciho cislo s danym indexem
  def [](n)
    @idx = n
    current
  end

private

  def fib(n)
    return @init[n] if n < 2
    return fib(n-2) + fib(n-1)
  end
end