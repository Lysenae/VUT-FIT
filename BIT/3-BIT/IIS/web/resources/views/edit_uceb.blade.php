<!-- resources/views/edit_uceb.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
				Upravení učebny
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

<form method="POST" action="edit" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="roomID" class="col-sm-5 control-label">
		Název místnosti*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="roomID" id="roomID" class="form-control"
	    value="{{ old('roomID', $record['roomID']) }}" 
			placeholder="Název místnosti" disabled>
	</div>
</div>
<div class="form-group">
	<label for="oddeleni" class="col-sm-5 control-label">
		Oddělení*
	</label>	
<div class="col-sm-7">	
	 <select name="oddeleni" id="oddeleni" class="form-control">
		<option value="">--vyberte hodnotu--</option>
	@foreach ($dep as $row)
		<option value="{{ $row->oddID }}" {{ old('nazev', 
		    $record['nazev'] ) == $row->nazev ? "selected":"" }}>
		    {{ $row->nazev }}</option>
	@endforeach	
	</select>
</div>
</div>
<div class="form-group">
	<label for="lokace" class="col-sm-5 control-label">
		Lokace kanceláře
	</label>
<div class="col-sm-7">	
	    <input type="text" name="lokace" id="lokace" class="form-control" 
	        value="{{ old('lokace', $record['lokace']) }}" placeholder="Lokace">
	</div>
</div>
<div class="form-group">
	<label for="sirka" class="col-sm-5 control-label">
		Rozměry*
	</label>
<div class="col-sm-7">	
	 <input type="text" name="sirka"  id="sirka" class="form-control" 
		value="{{ old('sirka', $record['sirka']) }}" placeholder="Délka [m]">
	  <input type="text" name="delka" id="delka" class="form-control"
		value="{{ old('delka', $record['delka']) }}" placeholder="Šířka [m]">
 		<input type="text" name="vyska" id="vyska" class="form-control" 
		value="{{ old('vyska', $record['vyska']) }}" placeholder="Výška [m]">
	</div>
</div>
	
<div class="form-group">
	<label for="mist_k_sezeni" class="col-sm-5 control-label">
		Kapacita
	</label>
<div class="col-sm-7">	
	    <input type="text" name="mist_k_sezeni" id="mist_k_sezeni" class="form-control" 
	        value="{{ old('mist_k_sezeni', $record['mist_k_sezeni']) }}"
		 			placeholder="Kapacita">
	</div>
</div>

<div class="form-group">
	<label for="pocet_rad" class="col-sm-5 control-label">
		Počet řad
	</label>
<div class="col-sm-7">	
	    <input type="text" name="pocet_rad" id="pocet_rad" class="form-control" 
	        value="{{ old('pocet_rad', $record['pocet_rad']) }}"
		 			placeholder="Počet řad">
	</div>
</div>

	<div class="form-group">
	<label for="mist_na_radu" class="col-sm-5 control-label">
		Mist na řadu
	</label>
<div class="col-sm-7">	
	    <input type="text" name="mist_na_radu" id="mist_na_radu" class="form-control" 
	        value="{{ old('mist_na_radu', $record['mist_na_radu']) }}"
		 			placeholder="Mist na řadu">
	</div>
</div>

	<div class="form-group">
	<label for="bloky" class="col-sm-5 control-label">
	Počet bloků
	</label>
<div class="col-sm-7">	
	    <input type="text" name="bloky" id="bloky" class="form-control" 
	        value="{{ old('bloky', $record['bloky']) }}"
		 			placeholder="Počet bloků">
	</div>
</div>
	<div class="col-sm-offset-5 col-sm-7">
		* povinné údaje
	</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" name="submit" value="save" class="btn btn-default">
	Uložit</button>
	<button type="submit" name="submit" value="delete" class="btn btn-danger">
	<i class="fa fa-trash"></i>Smazat</button>
</div>
</form>
	    </div>
	</div>
    </div>
</div>
@endsection
