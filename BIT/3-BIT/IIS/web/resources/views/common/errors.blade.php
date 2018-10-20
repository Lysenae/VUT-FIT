@if (count($errors) > 0)
<!-- Form Error List -->
    <div class="alert alert-danger">
	<strong>Oooouha! něco se nepovedlo. :-(</strong>

	<br><br>

	<ul>
        @foreach ($errors->all() as $error)
	    <li>{{ $error }}</li>
	@endforeach
	</ul>
    </div>
@endif
