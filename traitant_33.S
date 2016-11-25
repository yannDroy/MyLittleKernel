    .text
# cette directive sert a rendre l'etiquette publique
    .globl traitant_IT_33
# debut du traitant 
traitant_IT_33:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call clavier_PIT
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
