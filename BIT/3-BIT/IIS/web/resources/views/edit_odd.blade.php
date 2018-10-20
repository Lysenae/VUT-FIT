<!-- resources/views/edit_odd.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Upravení oddělení
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form method="POST" action="edit" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="nazev" class="col-sm-5 control-label">
		Název*
	</label>
	<div class="col-sm-7">	
		<input type="text" name="nazev" id="nazev" class="form-control" 
		value="{{ old('nazev', $record['nazev']) }}" placeholder="Název místnosti">
	</div>
</div>
<div class="form-group">
	<label for="popis" class="col-sm-5 control-label">
		Popis
	</label>
	<div class="col-sm-7">	
	<textarea name="popis" id="popis" class="form-control"
		placeholder="Popis">{{ old('popis', $record['popis']) }}</textarea>
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
