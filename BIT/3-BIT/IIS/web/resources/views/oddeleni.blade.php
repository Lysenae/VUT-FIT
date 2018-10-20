@extends('layouts.app')

@section('content')

<div class="container">
    <div class="">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Seznam oddělení
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>#</th>
			<th>Název</th>
			<th>Popis</th>
			<th></th>
		    </thead>
		    <tbody>
		    @foreach ($dep as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->oddID }}
			</td>
			<td class="table-text">
			    {{ $row->nazev }}
			</td>
			<td class="table-text">
			    {{ $row->popis }}
			</td>
			<td>
			<a href="/oddeleni/{{ $row->oddID }}/show">upravit</a>
			</td>
		    </tr>
		    @endforeach
		    </tbody>
		</table>
		</div>
		<form action="/oddeleni" method="POST" class="form-horizontal">
		    {{ csrf_field() }}

		    <!-- Add employee Button -->
		    <div class="form-group">
			<div class="col-sm-offset-0 col-sm-6">
			    <button type="submit" class="btn btn-default">
				<i class="fa fa-plus"></i> Přidat oddělení</button>
			</div>
		    </div>
		</form>
	    </div>
	</div>
</div>

    <!-- TODO: Current employees -->
@endsection
