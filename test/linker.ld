ENTRY(start)

SECTIONS {
	. = 0;
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
}
