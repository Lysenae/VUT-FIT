<!-- resources/views/edit_kanc.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
				Upravení kanceláře
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
	<label for="el_zasuvky" class="col-sm-5 control-label">
		Elektrických zásuvek*
	</label>
	<div class="col-sm-7">	
		<input type="text" name="el_zasuvky" id="el_zasuvky" class="form-control" 
	    value="{{ old('el_zasuvky', $record['el_zasuvky']) }}" 
			placeholder="Elektrických zásuvek">
	</div>
</div>

<div class="form-group">
	<label for="eth_zasuvky" class="col-sm-5 control-label">
		Ethernetové zásuvky
	</label>
	<div class="col-sm-7">
		<input type="text" name="eth_zasuvky" id="eth_zasuvky" class="form-control" 
	    value="{{ old('eth_zasuvky', $record['eth_zasuvky']) }}"
			placeholder="Ethernetové zásuvky">
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
