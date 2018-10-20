<!-- resources/views/edit_vt.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Upravení zařízení
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form action="edit" method="POST" class="form-horizontal">
    {!! csrf_field() !!}
<div class="form-group">
	<label for="umisteno" class="col-sm-5 control-label">
	Umístění*
	</label>
	<div class="col-sm-7">	
	    <select name="umisteno" id="umisteno" class="form-control">
		<option value="">--vyberte hodnotu--</option>
	@foreach ($umisteno as $row)
		<option value="{{ $row->roomID }}" {{ old('umisteno', $record['umisteno']) == 
		    $row->roomID ? "selected":"" }}>{{ $row->roomID }}</option>   
	@endforeach	
	    </select>	    
	</div>
</div>
<div class="form-group">
	<label for="zodpovida" class="col-sm-5 control-label">
	Zodpovídá*
	</label>
	<div class="col-sm-7">	
	    <select name="zodpovida" id="zodpovida" class="form-control">
		<option value="">--vyberte hodnotu--</option>
	@foreach ($zodpovida as $row)
		<option value="{{ $row->zamID }}" {{ old('zodpovida', $record['zodpovida']) == 
		    $row->zamID ? "selected":"" }} >{{ $row->prijmeni }} {{ $row->jmeno }}</option>   
	@endforeach	
	    </select>	    
	</div>
</div>
<div class="form-group">
	<label for="znacka" class="col-sm-5 control-label">
	Značka*	
	</label>
	<div class="col-sm-7">	
	<input type="text" name="znacka" id="znacka" class="form-control"
	    value="{{ old('znacka', $record['znacka']) }}" placeholder="Značka">
	</div>
</div>
<div class="form-group">
	<label for="model" class="col-sm-5 control-label">
	Model*
	</label>
	<div class="col-sm-7">	
	<input type="text" name="model" id="model" class="form-control"
	    value="{{ old('model', $record['model']) }}" placeholder="Model">
	</div>
</div>
<div class="form-group">
	<label for="cena" class="col-sm-5 control-label">
	Cena
	</label>
	<div class="col-sm-7">	
	<input type="text" name="cena" id="cena" class="form-control"
	    value="{{ old('cena', $record['cena']) }}" placeholder="Cena">
	</div>
</div>
<div class="form-group">
	<label for="specifikace" class="col-sm-5 control-label">
	Specifikace
	</label>
	<div class="col-sm-7">	
	<textarea name="specifikace" id="specifikace" class="form-control"
	    placeholder="Specifikace">{{ old('specifikace', $record['specifikace']) }}</textarea>
	</div>
</div>
<div class="form-group">
	<label for="stav" class="col-sm-5 control-label">
	Stav
	</label>
	<div class="col-sm-7">	
	<textarea name="stav" id="stav" class="form-control"
	    placeholder="Stav">{{ old('stav', $record['stav']) }}</textarea>
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
