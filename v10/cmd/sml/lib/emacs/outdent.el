;; outdent.el  

;; binds meta-tab to a function that reduces indentation to that of the next previous
;; line with smaller indentation.  Can be used repeatedly to "outdent" several levels.
;; Must be loaded after sml-mode.el, which defines sml-mode-map.

(defun back-to-outer-indent ()
  "Unindents out to the next outer level of indentation. Bound to \e\t"
  (interactive)
  (let ((start-column (current-column))
	(indent (current-column)))
    (if (> start-column 0)
	(progn
	  (save-excursion
	    (while (>= indent start-column)
	      (if (re-search-backward "^[^\n]" nil t)
		  (setq indent (current-indentation))
		(setq indent 0))))
	  (backward-delete-char-untabify (- start-column indent))))))


(define-key sml-mode-map "\e\t" 'back-to-outer-indent)
