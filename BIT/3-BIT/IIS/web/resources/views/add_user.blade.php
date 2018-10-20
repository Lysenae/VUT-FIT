<!-- resources/views/add_user.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Přidání zaměstnance
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form method="POST" action="/add_user" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="pracoviste" class="col-sm-5 control-label">
	Pracoviště*
	</label>
	<div class="col-sm-7">	
	    <select name="pracoviste" id="pracoviste" class="form-control" >
		<option value="">--vyberte hodnotu--</option>
	@foreach ($pracoviste as $row)
		<option value="{{ $row->roomID }}" {{ old('pracoviste') == 
		    $row->roomID ? "selected":"" }} >{{ $row->roomID }}</option>   
		    @endforeach
		    </select>
	</div>
</div>	    
<div class="form-group">
	<label for="pozice" class="col-sm-5 control-label">
	Pozice*
	</label>
	<div class="col-sm-7">	
	    <select name="pozice" id="pozice" class="form-control" >
		<option value="">--vyberte hodnotu--</option>
	@foreach ($prava as $row)
		<option value="{{ $row->pravaID }}" {{ old('pozice') == 
		    $row->pravaID ? "selected":"" }} >{{ $row->popis }}</option>
		    @endforeach
		    </select>
	</div>
</div>	    
<div class="form-group">
	<label for="login" class="col-sm-5 control-label">
	Login*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="login" id="login" class="form-control" 
	    value="{{ old('login') }}" placeholder="Login">
	</div>
</div>	    
	
<div class="form-group">
	<label for="jmeno" class="col-sm-5 control-label">
	    Jméno*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="jmeno" id="jmeno" class="form-control" 
	    value="{{ old('jmeno') }}" placeholder="Jméno">
	</div>
</div>	    
<div class="form-group">
	<label for="prijmeni" class="col-sm-5 control-label">
	    Příjmení*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="prijmeni" id="prijmeni" class="form-control"i
	    value="{{ old('prijmeni') }}" placeholder="Příjmení">
	</div>
</div>	    
<div class="form-group">
	<label for="date" class="col-sm-5 control-label">
	    Datum narození
	</label>
	<div class="col-sm-7">	
	<input type="date" name="datum_narozeni" id="date" class="form-control" 
	    value="{{ old('datum_narozeni') }}" placeholder="Datum narození" >
	</div>
</div>	    
<div class="form-group">
	<label for="mobil" class="col-sm-5 control-label">
	    Mobil
	</label>
	<div class="col-sm-7">	
	<input type="text" name="mobil" id="mobil" class="form-control"
	    value="{{ old('mobil') }}" placeholder="Mobil">
	</div>
</div>	    
<div class="form-group">
	<label for="telefon_kancelar" class="col-sm-5 control-label">
	    Telefon kancelář
	</label>
	<div class="col-sm-7">	
	<input type="text" name="telefon_kancelar" id="telefon_kancelar"
	    class="form-control" value="{{ old('telefon_kancelar') }}"
	    placeholder="Telefon kancelář">
	</div>
</div>	    
<div class="form-group">
	<label for="mail" class="col-sm-5 control-label">
	    E-mail
	</label>
	<div class="col-sm-7">	
	<input type="email" name="mail" id="mail" class="form-control" 
	    value="{{ old('mail') }}" placeholder="E-mail">
	</div>
</div>	    
<div class="form-group">
	<label for="password" class="col-sm-5 control-label">
	    Heslo*
	</label>
	<div class="col-sm-7">	
	<input type="password" name="password" id="password" class="form-control"
	    placeholder="Heslo">
	</div>
</div>	    
<div class="form-group">
	<label for="password_confirmation" class="col-sm-5 control-label">
	    Potvrzení hesla
	</label>
	<div class="col-sm-7">	
	<input type="password" name="password_confirmation"
	    id="password_confirmation" class="form-control"
	    placeholder="Potvrzení hesla">
	</div>
</div>	    


<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" class="btn btn-default">
	<i class="fa fa-plus"></i>Přidat uživatele</button>
</div>
    </form>

	    </div>
	</div>
    </div>
</div>
@endsection
