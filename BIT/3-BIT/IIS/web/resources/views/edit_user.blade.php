<!-- resources/views/add_user.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Upravení zaměstnance
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form method="POST" action="edit" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="pracoviste" class="col-sm-5 control-label">
	Pracoviště*
	</label>
	<div class="col-sm-7">	
	    <select name="pracoviste" id="pracoviste" class="form-control" >
		<option value="">--vyberte hodnotu--</option>
		@foreach ($pracoviste as $row)
		<option value="{{ $row->roomID }}" {{ old('pracoviste',
		    $record['pracoviste']) == $row->roomID ? "selected":"" }} >
		    {{ $row->roomID }}
		</option>   
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
		<option value="{{ $row->pravaID }}" {{ old('pozice',
		    $record['prava']) == $row->pravaID ? "selected":"" }}>
		    {{ $row->popis }}
		</option>	
	@endforeach	
	</select>
	</div>
</div>
<div class="form-group">
	<label for="jmeno" class="col-sm-5 control-label">
	    Jméno*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="jmeno" id="jmeno" class="form-control" 
	    value="{{ old('jmeno', $record['jmeno']) }}" placeholder="Jméno">
	</div>
</div>	    
<div class="form-group">
	<label for="prijmeni" class="col-sm-5 control-label">
	    Příjmení*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="prijmeni" id="prijmeni" class="form-control"i
	    value="{{ old('prijmeni', $record['prijmeni']) }}" placeholder="Příjmení">
	</div>
</div>	    
<div class="form-group">
	<label for="date" class="col-sm-5 control-label">
	    Datum narození
	</label>
	<div class="col-sm-7">	
	<input type="date" name="datum_narozeni" id="date" class="form-control" 
	    value="{{ old('datum_narozeni', $record['datum_narozeni']) }}"
	     placeholder="Datum narození" >
	</div>
</div>	    
<div class="form-group">
	<label for="mobil" class="col-sm-5 control-label">
	    Mobil
	</label>
	<div class="col-sm-7">	
	<input type="text" name="mobil" id="mobil" class="form-control"
	    value="{{ old('mobil', $record['mobil']) }}" placeholder="Mobil">
	</div>
</div>	    
<div class="form-group">
	<label for="telefon_kancelar" class="col-sm-5 control-label">
	    Telefon kancelář
	</label>
	<div class="col-sm-7">	
	<input type="text" name="telefon_kancelar" id="telefon_kancelar"
	    class="form-control" 
	    value="{{ old('telefon_kancelar', $record['telefon_kancelar']) }}"
	    placeholder="Telefon kancelář">
	</div>
</div>	    
<div class="form-group">
	<label for="mail" class="col-sm-5 control-label">
	    E-mail
	</label>
	<div class="col-sm-7">	
	<input type="email" name="mail" id="mail" class="form-control" 
	    value="{{ old('mail', $record['mail']) }}" placeholder="E-mail">
	</div>
</div>	    

<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" name="submit" class="btn btn-default" value="save">
	Uložit</button>
	<button type="submit" name="submit" class="btn btn-danger" value="delete">
	<i class="fa fa-trash"></i>Smazat</button>
</div>
	</form>
	    </div>
	</div>
    </div>
</div>
@endsection
