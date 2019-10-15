c0=3.1622e18
cl=3.1622e18
trap_per_free=1e3
N=3.1622e22
time_scaling=1
epsilon=4000
dt=2
alpha_t=1e15
alpha_r=1e13

[GlobalParams]
  lm_sign = -1
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 100
  xmax = 1
  elem_type = EDGE3
[]

[Problem]
  kernel_coverage_check = false
#   type = ReferenceResidualProblem
#   extra_tag_vectors = 'ref'
#   reference_vector = 'ref'
#   solution_variables = 'mobile trapped'
[]

[Debug]
  show_var_residual_norms = true
[]

[Variables]
  [mobile]
    order = SECOND
  []
  [trapped]
    # order = SECOND
  []
  [mobile_lm]
    # order = SECOND
  []
  [trapped_lm]
    # order = SECOND
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxVariables]
  [empty_sites][]
  [scaled_empty_sites][]
  [trapped_sites][]
[]

[AuxKernels]
  [empty_sites]
    variable = empty_sites
    type = EmptySitesAux
    N = ${fparse N / cl}
    Ct0 = .1
    trap_per_free = ${trap_per_free}
    trapped_concentration_variables = trapped
  []
  [scaled_empty]
    variable = scaled_empty_sites
    type = NormalizationAux
    normal_factor = ${cl}
    source_variable = empty_sites
  []
  [trapped_sites]
    variable = trapped_sites
    type = NormalizationAux
    normal_factor = ${trap_per_free}
    source_variable = trapped
  []
[]

[Kernels]
  # mobile kernels
  [./diff]
    type = MatDiffusion
    variable = mobile
    diffusivity = ${fparse 1. / time_scaling}
    # extra_vector_tags = ref
  [../]
  [diff_lm]
    type = LMDiffusion
    variable = mobile_lm
    v = mobile
    diffusivity = ${fparse 1. / time_scaling}
  []
  [./time]
    type = TimeDerivativeLM
    variable = mobile
    lm_variable = mobile_lm
    # extra_vector_tags = ref
  [../]
  [coupled_time]
    type = ScaledCoupledTimeDerivativeLM
    variable = mobile
    v = trapped
    factor = ${trap_per_free}
    lm_variable = mobile_lm
    # extra_vector_tags = ref
  []
  [mobile_lm_source]
    type = CoupledForceLM
    variable = mobile
    v = mobile_lm
    lm_variable = mobile_lm
    coef = 1
  []

  # trapping kernels
  [time_trapped]
    type = TimeDerivativeLM
    variable = trapped
    lm_variable = trapped_lm
  []
  [trapping]
    type = TrappingLM
    variable = trapped
    alpha_t = ${fparse alpha_t / time_scaling}
    N = ${fparse N / cl}
    Ct0 = 0.1
    mobile = 'mobile'
    trap_per_free = ${trap_per_free}
    # extra_vector_tags = ref
    lm_variable = trapped_lm
  []
  [release]
    type = ReleasingLM
    alpha_r = ${fparse alpha_r / time_scaling}
    temp = 1000
    trapping_energy = ${epsilon}
    variable = trapped
    lm_variable = trapped_lm
  []
  [trapped_lm_source]
    type = CoupledForceLM
    variable = trapped
    v = trapped_lm
    lm_variable = trapped_lm
    coef = 1e10
  []

  # constraint kernels
  [mobile_constraint]
    type = RequirePositiveNCP
    variable = mobile_lm
    v = mobile
    coef = 1
  []
  [trapped_constraint]
    type = RequirePositiveNCP
    variable = trapped_lm
    v = trapped
    coef = 1e10
  []
[]

[BCs]
  [left]
    type = PresetBC
    variable = mobile
    value = ${fparse c0 / cl}
    boundary = left
  []
  [right]
    type = PresetBC
    variable = mobile
    value = 0
    boundary = right
  []
[]

[Postprocessors]
  [outflux]
    type = SideFluxAverage
    boundary = 'right'
    diffusivity = 1
    variable = mobile
  []
  [scaled_outflux]
    type = ScalePostprocessor
    value = outflux
    scaling_factor = ${cl}
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  end_time = 1000
  dt = ${dt}
  dtmin = ${dt}
  solve_type = NEWTON
  line_search = 'none'
  automatic_scaling = true
  verbose = true
  compute_scaling_once = false
[]

[Outputs]
  exodus = true
  csv = true
  [dof]
    type = DOFMap
    execute_on = initial
  []
  perf_graph = true
[]
